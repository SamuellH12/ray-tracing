#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "../ObjReader.cpp"
#include "../Matrix.cpp"
#include "Objeto.cpp"
#include <vector>
#include <tuple>

class malha : public objeto {
private:
    // // // point pos; vetor color;
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
        calc_boundbox();
    }

    Intersection get_intersection(ray &r) override{ 
        if(!boundbox_intersection(r)) return Intersection();
        Intersection ans;

        for(int i=0; i<faces.size(); i++)
            ans = min<Intersection>(ans, get_face_intersection(i, r));

        return ans;
    }

    void affine_transform(matrix<4, 4> m){
        for(auto &p : vertices) p = m * p;
        for(auto &v : normals)  v = m * v;
        calc_boundbox();
    }

    point get_centroid(){ return centroid; }


// Otimização
// cria uma bound box em volta do objeto
// primeiro testa a interseção com a bound box
// se não existir ignora o objeto
private:
    point boundbox1;
    point boundbox2;
    point centroid;

    void calc_boundbox(){
        double xn, yn, zn, xm, ym, zm;
        xn = yn = zn = std::numeric_limits<double>::infinity();
        xm = ym = zm = -xn;

        for(auto p : vertices)
        {
            xn = min(xn, p.getX());
            yn = min(yn, p.getY());
            zn = min(zn, p.getZ());
            
            xm = max(xm, p.getX());
            ym = max(ym, p.getY());
            zm = max(zm, p.getZ());

            centroid = centroid + (p - point());
        }

        centroid = point() + ((centroid - point()) / vertices.size());

        boundbox1 = point(xn, yn, zn);
        boundbox2 = point(xm, ym, zm);
    }

    bool boundbox_intersection(ray &r){
        for(int __=0; __<2; __++, swap(boundbox1, boundbox2))
        {
            // plano X
            double n = r.get_direction().getX();
            if(abs(n) > 1e-8){
                double t = (boundbox1 - r.get_origin()).getX() / n;
                if(t > 0) {    
                    point i = r.get_origin() + r.get_direction() * t;
                    point&a = boundbox1;
                    point c (a.getX(), boundbox2.getY(), boundbox2.getZ());
                    point b (a.getX(), a.getY(), c.getZ());
                    point d (a.getX(), c.getY(), a.getZ());

                    int modulo = round(((b-a) % (i - a)).normalized().getX());
                    if( ((c-b) % (i - b)).normalized().getX()*modulo > 0 
                    &&  ((d-c) % (i - c)).normalized().getX()*modulo > 0 
                    &&  ((a-d) % (i - d)).normalized().getX()*modulo > 0 )
                        return true;
                }
            }

            // plano Y
            n = r.get_direction().getY();
            if(abs(n) > 1e-8){
                double t = (boundbox1 - r.get_origin()).getY() / n;
                if(t > 0) {
                    point i = r.get_origin() + r.get_direction() * t;
                    point&a = boundbox1;
                    point c (boundbox2.getX(), a.getY(), boundbox2.getZ());
                    point b (a.getX(), a.getY(), c.getZ());
                    point d (c.getX(), a.getY(), a.getZ());

                    int modulo = round(((b-a) % (i - a)).normalized().getY());
                    if( ((c-b) % (i - b)).normalized().getY()*modulo > 0 
                    &&  ((d-c) % (i - c)).normalized().getY()*modulo > 0 
                    &&  ((a-d) % (i - d)).normalized().getY()*modulo > 0 )
                        return true;
                }
            }

            // Plano Z
            n = r.get_direction().getZ();
            if(abs(n) > 1e-8){
                double t = (boundbox1 - r.get_origin()).getZ() / n;
                if(t > 0) {
                    point i = r.get_origin() + r.get_direction() * t;
                    point&a = boundbox1;
                    point c (boundbox2.getX(), boundbox2.getY(), a.getZ());
                    point b (c.getX(), a.getY(), a.getZ());
                    point d (a.getX(), c.getY(), a.getZ());

                    int modulo = round(((b-a) % (i - a)).normalized().getZ());
                    if( ((c-b) % (i - b)).normalized().getZ()*modulo > 0 
                    &&  ((d-c) % (i - c)).normalized().getZ()*modulo > 0 
                    &&  ((a-d) % (i - d)).normalized().getZ()*modulo > 0 )
                        return true;
                }
            }

        }

        return false;
    };
};