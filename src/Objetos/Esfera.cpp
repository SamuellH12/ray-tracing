#ifndef ESFERAHEADER
#define ESFERAHEADER
#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "Objeto.cpp"
#include <vector>
#include <tuple>

class esfera : public objeto {
private:
    double h;

public:
    esfera(){}
    esfera(point pos, double raio, Color kd) : objeto(pos, kd), h(raio) {}

    double dist_intersection(ray &r) override{
        vetor oc = r.get_origin() - pos;
        double a = r.get_direction() * r.get_direction();
        double b = 2.0 * (oc * r.get_direction());
        double c = oc * oc - h*h;
        double dlt = b*b - 4.0*a*c;

        if(dlt < 0.0) return DOUBLEINF;

        double t = (-b - sqrt(dlt)) / (2.0 * a);
        if(t < 0) t = (-b + sqrt(dlt)) / (2.0 * a);

        return t;
    }


    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade = MAXREC) override{
        vetor oc = r.get_origin() - pos;
        double a = r.get_direction() * r.get_direction();
        double b = 2.0 * (oc * r.get_direction());
        double c = oc * oc - h*h;
        double dlt = b*b - 4.0*a*c;

        if(dlt < 0.0) return Intersection();

        double raiz = sqrt(dlt);
        double t = (-b - raiz) / (2.0*a);
        if(t < 0) t = (-b + raiz) / (2.0*a);
        if(t < 0) return Intersection();

        point inter = r.get_origin() + (r.get_direction()*t);  
        vetor normal = (inter - pos).normalized();

        Color cl = get_color(r, inter, normal, Ia, luzes, objetos, profundidade);

        return Intersection(t, normal, cl);
    }
};

#endif