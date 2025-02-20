#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "../ObjReader.cpp"
#include "../Matrix.cpp"
#include "Objeto.cpp"
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
        point &b = vertices[f.verticeIndice[1]];
        point &c = vertices[f.verticeIndice[2]];
        vetor normal = ((b-a) % (c-a)).normalized();

        double n = normal * r.get_direction();

        if(abs(n) < 1e-8) return Intersection();

        double d = normal * (a - r.get_origin());
        double t = (d/n);

        if(t <= 0) return Intersection();
        //tem intersecao com o plano
        //a intersecao esta dentro do triangulo?

        point i = r.get_origin() + r.get_direction() * t;

        if( ((b-a) % (i - a)) * normal < 0 ) return Intersection();
        if( ((c-b) % (i - b)) * normal < 0 ) return Intersection();
        if( ((a-c) % (i - c)) * normal < 0 ) return Intersection();


        Color cl = color * std::max(0.0, normal * (r.get_origin() - i).normalized());
        return Intersection(t, normal, cl);
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

    Intersection get_intersection(ray &r) override{ 
        Intersection ans;

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


        return ans;
    }

    void affine_transform(matrix<4, 4> m){
        for(auto &p : vertices) p = m * p;
        for(auto &v : normals)  v = m * v;
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
    std::deque<std::tuple<int, int, int>> q;


    void calc_tree(){
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
            centroid = centroid + (p - point());
        centroid = point() + ((centroid - point()) / vertices.size());
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

    bool boundbox_intersection(ray &r, point &bx1, point &bx2){
        for(int __=0; __<2; __++, swap(bx1, bx2))
        {
            // plano X
            double n = r.get_direction().getX();
            if(abs(n) > 1e-6){
                double t = (bx1 - r.get_origin()).getX() / n;
                if(t > 0) {    
                    point i = r.get_origin() + r.get_direction() * t;
                    point&a = bx1;
                    point c (a.getX(), bx2.getY(), bx2.getZ());
                    point b (a.getX(), a.getY(), c.getZ());
                    point d (a.getX(), c.getY(), a.getZ());

                    int modulo = ((b-a) % (i - a)).getX() > 0 ? 1 : -1;
                    if( ((c-b) % (i - b)).getX()*modulo > 0 
                    &&  ((d-c) % (i - c)).getX()*modulo > 0 
                    &&  ((a-d) % (i - d)).getX()*modulo > 0 )
                        return true;
                }
            }

            // plano Y
            n = r.get_direction().getY();
            if(abs(n) > 1e-6){
                double t = (bx1 - r.get_origin()).getY() / n;
                if(t > 0) {
                    point i = r.get_origin() + r.get_direction() * t;
                    point&a = bx1;
                    point c (bx2.getX(), a.getY(), bx2.getZ());
                    point b (a.getX(), a.getY(), c.getZ());
                    point d (c.getX(), a.getY(), a.getZ());

                    int modulo = ((b-a) % (i - a)).getY() > 0 ? 1 : -1;
                    if( ((c-b) % (i - b)).getY()*modulo > 0 
                    &&  ((d-c) % (i - c)).getY()*modulo > 0 
                    &&  ((a-d) % (i - d)).getY()*modulo > 0 )
                        return true;
                }
            }

            // Plano Z
            n = r.get_direction().getZ();
            if(abs(n) > 1e-6){
                double t = (bx1 - r.get_origin()).getZ() / n;
                if(t > 0) {
                    point i = r.get_origin() + r.get_direction() * t;
                    point&a = bx1;
                    point c (bx2.getX(), bx2.getY(), a.getZ());
                    point b (c.getX(), a.getY(), a.getZ());
                    point d (a.getX(), c.getY(), a.getZ());

                    int modulo = ((b-a) % (i - a)).getZ() > 0 ? 1 : -1;
                    if( ((c-b) % (i - b)).getZ()*modulo > 0 
                    &&  ((d-c) % (i - c)).getZ()*modulo > 0 
                    &&  ((a-d) % (i - d)).getZ()*modulo > 0 )
                        return true;
                }
            }

        }

        return false;
    }
};