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

    bool has_intersection(ray &r, double tmax = std::numeric_limits<double>::infinity()) override{
        vetor oc = r.get_origin() - pos;
        double a = r.get_direction() * r.get_direction();
        double b = 2.0 * (oc * r.get_direction());
        double c = oc * oc - h*h;
        double dlt = b*b - 4.0*a*c;

        if(dlt < 0.0) return false;

        double t = (-b - sqrt(dlt)) / (2.0 * a);
        if(t < 0) t = (-b + sqrt(dlt)) / (2.0 * a);

        return t >= 0.0 && t <= tmax;
    }


    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos) override{
        vetor oc = r.get_origin() - pos;
        double a = r.get_direction() * r.get_direction();
        double b = 2.0 * (oc * r.get_direction());
        double c = oc * oc - h*h;
        double dlt = b*b - 4.0*a*c;

        if(dlt < 0.0) return Intersection();

        double t = (-b - sqrt(dlt)) / (2.0 * a);
        if(t < 0) t = (-b + sqrt(dlt)) / (2.0*a);

        point inter = r.get_origin() + (r.get_direction()*t);  
        vetor normal = inter - pos;

        Color cl = get_color(r, inter, normal, Ia, luzes, objetos);

        return t >= 0.0 ? Intersection(t, normal, cl) : Intersection();
    }
};

#endif