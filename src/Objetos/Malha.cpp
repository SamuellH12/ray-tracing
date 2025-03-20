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

    Intersection get_face_intersection(int id_face, ray &r){
        Face &f = faces[id_face];
        point &a = vertices[f.verticeIndice[0]];
        vetor normal = f.normal;

        double n = normal * r.get_direction();

        if( n == 0.0) return Intersection();

        double d = normal * (a - r.get_origin());
        double t = (d/n);

        if(t < 0) return Intersection();
        //tem intersecao com o plano
        //a intersecao esta dentro do triangulo?

        point i = r.get_origin() + r.get_direction() * t;
        point &b = vertices[f.verticeIndice[1]];
        point &c = vertices[f.verticeIndice[2]];

        if( ((b-a) % (i - a)) * normal < 0 ) return Intersection();
        if( ((c-b) % (i - b)) * normal < 0 ) return Intersection();
        if( ((a-c) % (i - c)) * normal < 0 ) return Intersection();

        // Color cl = f.kd * max(0.0, min(normal * r.get_direction() * -1.0 + 0.15, 1.0));
        // Color cl = f.get_color(r, i, normal, Ia, luzes, objetos);
        return Intersection(t, normal, Color(), id_face);
    }

    Color get_face_color(int id_face, ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade){
        Face &f = faces[id_face];
        point &a = vertices[f.verticeIndice[0]];
        vetor normal = f.normal;

        double n = normal * r.get_direction();
        double d = normal * (a - r.get_origin());
        double t = (d/n);
        point i = r.get_point(t);

        return f.get_color(r, i, normal, Ia, luzes, objetos, profundidade);
    }

    double dist_face_intersection(int id_face, ray &r){
        Face &f = faces[id_face];
        point &a = vertices[f.verticeIndice[0]];
        vetor normal = f.normal;

        double n = normal * r.get_direction();

        if(n == 0.0) return DOUBLEINF;

        double d = normal * (a - r.get_origin());
        double t = (d/n);

        if(t < 0.0) return DOUBLEINF;

        point i = r.get_origin() + r.get_direction() * t;
        point &b = vertices[f.verticeIndice[1]];
        point &c = vertices[f.verticeIndice[2]];

        if( ((b-a) % (i - a)) * normal < 0 ) return DOUBLEINF;
        if( ((c-b) % (i - b)) * normal < 0 ) return DOUBLEINF;
        if( ((a-c) % (i - c)) * normal < 0 ) return DOUBLEINF;

        return t;
    }


public:
    malha(){}
    malha(objReader &obj) : objeto(point(), obj.getKd()) {
        vertices = obj.getVertices();
        normals = obj.getNormals();
        faces = obj.getFaces();
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
                    t = std::min(t, dist_face_intersection(i, r2));
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
                    if(dist_face_intersection(i, r2) < tmax)
                        return true;
        }

        return false;
    }

    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade = MAXREC) override{ 
        Intersection ans;
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
            else for(int i=l; i<=r; i++)
                ans = min<Intersection>(ans, get_face_intersection(i, r2));
        }

        if(ans.metadata != -1) ans.color = get_face_color(ans.metadata, r2, Ia, luzes, objetos, profundidade);

        return ans;
    }

    void affine_transform(matrix<4, 4> m){
        for(auto &p : vertices) p = m * p;
        for(auto &v : normals)  v = m * v;
        for(auto &f : faces) f.recalc_normal(vertices);
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
    point centroid;
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
                    if(sortby == 1) return vertices[a.verticeIndice[0]].getX() < vertices[b.verticeIndice[0]].getX();
                    if(sortby == 2) return vertices[a.verticeIndice[0]].getY() < vertices[b.verticeIndice[0]].getY();
                    return vertices[a.verticeIndice[0]].getZ() < vertices[b.verticeIndice[0]].getZ();
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
            auto &p = vertices[faces[i].verticeIndice[j]];
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