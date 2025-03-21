#ifndef FACEHEADER
#define FACEHEADER
#include <vector>
#include <array>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "Objeto.cpp"

class Face : public objeto {
    public:
    std::array<point*, 3> vertices;
    int verticeIndice[3];
    int normalIndice[3];
    vetor normal;

    Face(){}

    void recalc_normal(){
        normal = (*vertices[1] - *vertices[0]) % 
                 (*vertices[2] - *vertices[0]);
        normal = normal.normalized();
    }
    
    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade = MAXREC) override{ 
        point &a = *vertices[0];
        double n = normal * r.get_direction();

        if( n == 0.0) return Intersection();

        double d = normal * (a - r.get_origin());
        double t = (d/n);

        if(t < 0) return Intersection();

        point i = r.get_point(t);
        point &b = *vertices[1];
        point &c = *vertices[2];

        if( ((b-a) % (i - a)) * normal < 0 ) return Intersection();
        if( ((c-b) % (i - b)) * normal < 0 ) return Intersection();
        if( ((a-c) % (i - c)) * normal < 0 ) return Intersection();

        Color cl = get_color(r, i, normal, Ia, luzes, objetos, profundidade);
        return Intersection(t, normal, cl);
    }

    double dist_intersection(ray &r) override {
        point &a = *vertices[0];
        double n = normal * r.get_direction();

        if(n == 0.0) return DOUBLEINF;

        double d = normal * (a - r.get_origin());
        double t = (d/n);

        if(t < 0.0) return DOUBLEINF;

        point i = r.get_point(t);
        point &b = *vertices[1];
        point &c = *vertices[2];

        if( ((b-a) % (i - a)) * normal < 0 ) return DOUBLEINF;
        if( ((c-b) % (i - b)) * normal < 0 ) return DOUBLEINF;
        if( ((a-c) % (i - c)) * normal < 0 ) return DOUBLEINF;

        return t;
    }
};


#endif