#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "Objeto.cpp"
#include <vector>
#include <tuple>

const double EPSILON = 1e-10;

int cmp(double a, double b){
    if(std::abs(a - b) < EPSILON) return 0;
    return (a < b ? -1 : 1);
}

class esfera : public objeto {
private:
double h;

public:

esfera(){}
esfera(point pos, double raio, vetor color) : objeto(pos, color), h(raio) {}

bool has_intersection(ray r) override { 
    vetor oc = r.get_origin() - pos;
    double a = r.get_direction() * r.get_direction();
    double b = 2.0 * (oc * r.get_direction());
    double c = oc * oc - h*h;
    double dlt = b*b - 4.0*a*c;

    if(cmp(dlt, 0) == -1) return false;

    double t = (-b - sqrt(dlt)) / (2.0 * a);

    if(cmp(t, 0) == -1) return false;
    return true;
}
//position // normal // color
std::tuple<point, vetor, vetor> get_intersection(ray r) override{
    vetor oc = r.get_origin() - pos;
    double a = r.get_direction() * r.get_direction();
    double b = 2.0 * (oc * r.get_direction());
    double c = oc * oc - h*h;
    double dlt = b*b - 4.0*a*c;

    // if(cmp(dlt, 0) == -1) return false;

    double t = (-b - sqrt(dlt)) / (2.0 * a);

    // if(cmp(t, 0) == -1) return false;

    point inter = r.get_origin() + (r.get_direction()*t);  
    vetor normal = inter - pos;
    return std::tuple(inter, normal, color);
}
};