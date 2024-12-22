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
esfera(point pos, double raio, vetor color) : objeto(pos, color), h(raio) {}

bool has_intersection(ray r) override { 
    vetor D = r.get_direction();
    point O = r.get_origin();
    vetor L = O - pos;
    double a = D*D;
    double b = 2.0*(L*D);
    double c = L*L - h*h;

    double dlt = b*b - 4*a*c;

    if(dlt <= 0.000001) return false; // < ou <= ???    
    
    double t = (-b - sqrt(dlt)) / (2.0*a);

    if(t < 0) t = (-b + sqrt(dlt)) / (2.0*a);

    return t > 0;
}
//position // normal // color
std::tuple<point, vetor, vetor> get_intersection(ray r) override{
    vetor D = r.get_direction();
    point O = r.get_origin();
    vetor L = O - pos;
    double a = D*D;
    double b = 2.0*(L*D);
    double c = L*L - h*h;

    double dlt = b*b - 4*a*c;
    
    double t = (-b - sqrt(dlt)) / (2.0*a);
    if(t < 0) t = (-b + sqrt(dlt)) / (2.0*a);

    point inter = r.get_origin() + (r.get_direction()*t);  
    vetor normal = inter - pos;
    return std::tuple(inter, normal, color);
}
};