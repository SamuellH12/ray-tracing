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

    double n = normal * r.get_direction();
    
    if(abs(n) <= 0.00001) return false;
    
    double d = normal * (pos - r.get_origin());
    double t = (d/n);

    return t > 0;
}

//position // normal // color
std::tuple<point, vetor, vetor> get_intersection(ray r) override{

    double n = normal * r.get_direction();
    double d = normal * (pos - r.get_origin());
    double t = (d/n);

    point inter = r.get_origin() + r.get_direction()*t;

    return std::tuple(inter, normal, color);
}
};