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
    esfera(point pos, double raio, Color color) : objeto(pos, color), h(raio) {}

    Intersection get_intersection(ray &r) override{ 
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
        vetor normal = inter - pos;

        double seno = (normal%r.get_direction()).norm() / (normal.norm() * (r.get_direction().norm()));
        Color cl = color*(1.0-abs(seno));

        return Intersection(t, normal, cl);
    }
};