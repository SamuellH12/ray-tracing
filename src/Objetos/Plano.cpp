#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "Objeto.cpp"
#include <vector>
#include <tuple>

class plano : public objeto {
private:
vetor normal;

public:

plano(){}
plano(point pos, vetor normal, vetor color) : objeto(pos, color), normal(normal) {}

bool has_intersection(ray r) override{ 
    vetor D = r.get_direction();
    point O = r.get_origin();

    vetor L = O - pos; 
    double d = normal * L;
    double n = normal * D;

    if(n == 0.0) return false;
    
    double t = -(d/n);
    return t > 0;
}

//position // normal // color
std::tuple<point, vetor, vetor> get_intersection(ray r) override{
    vetor D = r.get_direction();
    point O = r.get_origin();

    vetor L = O - pos; 
    double d = normal * L;
    double n = normal * D;
    
    double t = -(d/n);
    point inter = O + D*t;

    return std::tuple(inter, normal, color);
}
};