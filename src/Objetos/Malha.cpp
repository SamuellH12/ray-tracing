#ifndef MALHAHEADER
#define MALHAHEADER
#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "../ObjReader.cpp"
#include "../Matrix.cpp"
#include "Objeto.cpp"
#include "Face.cpp"
#include <vector>
#include <deque>
#include <tuple>
#include<algorithm>

class malha : public objeto {
private:
    std::vector<point> vertices;
    std::vector<vetor> normals;
    std::vector<Face> faces;
public:
    malha(){}

    // objeto(point pos, Color kd, Color ks, Color ka, Color ke, double ni, double d, double ns) : 
    malha(objReader &obj) : objeto(point(), obj.getKd(), obj.getKs(), obj.getKa(), obj.getKe(), obj.getNi(), obj.getD(), obj.getNs() ) {
        vertices = obj.getVertices();
        normals = obj.getNormals();
        faces = obj.getFaces();
        
        for(auto &f : faces)
            f.vertices[0] = &vertices[f.verticeIndice[0]],
            f.vertices[1] = &vertices[f.verticeIndice[1]],
            f.vertices[2] = &vertices[f.verticeIndice[2]],
            f.recalc_normal();

        calc_tree();
        calc_centroid();
    }

    double dist_intersection(ray &r) override{
        std::deque<std::tuple<int, int, int>> q;
        q.emplace_back(0, 0, (int)faces.size()-1);
        auto r2 = r;
        double t = DOUBLEINF;

        while(!q.empty())
        {
            auto [u, l, r] = q.front();
            q.pop_front();

            if(!boundbox_intersection(r2, boundboxes[u].first, boundboxes[u].second))
                continue;

            if(node_childs[u].first != -1)
            {
                q.emplace_back(node_childs[u].first, l, (l+r)/2);
                q.emplace_back(node_childs[u].second, (l+r)/2+1, r);
            }
            else 
                for(int i=l; i<=r; i++)
                    t = std::min(t, faces[i].dist_intersection(r2));
        }
        
        return t;
    }

    bool has_intersection(ray &r, double tmax = std::numeric_limits<double>::infinity()) override{
        std::deque<std::tuple<int, int, int>> q;
        q.emplace_back(0, 0, (int)faces.size()-1);
        auto r2 = r;

        while(!q.empty())
        {
            auto [u, l, r] = q.front();
            q.pop_front();

            if(!boundbox_intersection(r2, boundboxes[u].first, boundboxes[u].second))
                continue;

            if(node_childs[u].first != -1)
            {
                q.emplace_back(node_childs[u].first, l, (l+r)/2);
                q.emplace_back(node_childs[u].second, (l+r)/2+1, r);
            }
            else 
                for(int i=l; i<=r; i++)
                    if(faces[i].dist_intersection(r2) < tmax)
                        return true;
        }

        return false;
    }

    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade = MAXREC) override{ 
        std::deque<std::tuple<int, int, int>> q;
        Face* f = NULL;
        double dist = DOUBLEINF;

        q.emplace_back(0, 0, (int)faces.size()-1);
        auto r2 = r;

        while(!q.empty())
        {
            auto [u, l, r] = q.front();
            q.pop_front();

            if(!boundbox_intersection(r2, boundboxes[u].first, boundboxes[u].second))
                continue;

            if(node_childs[u].first != -1)
            {
                q.emplace_back(node_childs[u].first, l, (l+r)/2);
                q.emplace_back(node_childs[u].second, (l+r)/2+1, r);
            }
            else for(int i=l; i<=r; i++){
                double dt = faces[i].dist_intersection(r2);
                if(0.0 < dt && dt < dist)   
                    dist = dt,
                    f = &faces[i];
            }
        }

        if(f) return f->get_intersection(r2, Ia, luzes, objetos, profundidade);
        return Intersection();
    }

    void affine_transform(matrix<4, 4> m){
        for(auto &p : vertices) p = m * p;
        for(auto &v : normals)  v = m * v;
        for(auto &f : faces) f.recalc_normal();
        calc_tree();
        calc_centroid();
    }

    point get_centroid(){ return centroid; }


// Otimização
// cria uma bound box em volta do objeto
// primeiro testa a interseção com a bound box
// se não existir ignora o objeto
private:
    const int LIM_TREE_NODE = 32; //limite de faces por nó
    point &centroid = pos;
    std::vector<std::pair<point, point>> boundboxes;
    std::vector<std::pair<int, int>> node_childs;


    void calc_tree(){
        std::deque<std::tuple<int, int, int>> q;
        boundboxes.clear();
        node_childs.clear();
        
        q.emplace_back(0, 0, (int)faces.size()-1);
        boundboxes.emplace_back(point(), point());
        node_childs.emplace_back(-1, -1);

        while(!q.empty())
        {
            auto [u, l, r] = q.front();
            q.pop_front();

            boundboxes[u] = calc_boundbox(l, r);

            if(r-l+1 >= 2*LIM_TREE_NODE)
            {
                double xl =  boundboxes[u].second.getX() - boundboxes[u].first.getX();
                double yl =  boundboxes[u].second.getY() - boundboxes[u].first.getY();
                double zl =  boundboxes[u].second.getZ() - boundboxes[u].first.getZ();

                int sortby = 0;
                if(yl >= zl && yl >= xl) sortby = 1;
                if(zl >= yl && zl >= xl) sortby = 2;

                std::sort(std::begin(faces) + l, std::begin(faces) + r, [&](Face &a, Face &b){
                    if(sortby == 1) return a.vertices[0]->getX() < b.vertices[0]->getX();
                    if(sortby == 2) return a.vertices[0]->getY() < b.vertices[0]->getY();
                    return a.vertices[0]->getZ() < b.vertices[0]->getZ();
                });

                node_childs[u] = {node_childs.size(), node_childs.size()+1};

                q.emplace_back(node_childs.size(), l, (l+r)/2);
                boundboxes.emplace_back(point(), point());
                node_childs.emplace_back(-1, -1);

                q.emplace_back(node_childs.size(), (l+r)/2+1, r);
                boundboxes.emplace_back(point(), point());
                node_childs.emplace_back(-1, -1);
            }
        }
    }

    void calc_centroid(){
        centroid = point();
        for(auto p : vertices)
            centroid = centroid + p.to_vetor();
        centroid = point() + centroid.to_vetor() / vertices.size();
    }

    std::pair<point, point> calc_boundbox(int l, int r){
        double xn, yn, zn, xm, ym, zm;
        xn = yn = zn = std::numeric_limits<double>::infinity();
        xm = ym = zm = -xn;

        for(int i=l; i<=r; i++)
        for(int j=0; j<3; j++) 
        {
            auto &p = *faces[i].vertices[j];
            xn = min(xn, p.getX());
            yn = min(yn, p.getY());
            zn = min(zn, p.getZ());
            
            xm = max(xm, p.getX());
            ym = max(ym, p.getY());
            zm = max(zm, p.getZ());
        }

        return {point(xn, yn, zn), point(xm, ym, zm)};
    }
};
#endif