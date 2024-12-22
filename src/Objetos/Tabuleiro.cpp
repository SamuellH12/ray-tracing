#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "Objeto.cpp"
#include <vector>
#include <tuple>

class tabuleiro : public objeto {
private:
vetor normal;
vetor dir;
vetor ort;
vetor colorb;

public:

tabuleiro(){}
tabuleiro(point pos, vetor normal, vetor dir, vetor color, vetor colorb) : objeto(pos, color), normal(normal), dir(dir), colorb(colorb){
    dir = dir - (normal * ((normal*dir) / (normal*normal)));
    ort = (normal % dir).normalized() * sqrt(dir*dir);
}

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

    vetor cl = color;

    vetor oi = inter - pos;
    int x = ((oi*dir) / (dir*dir));
    int y = ((oi*ort) / (ort*ort));

    if(x%2 == y%2) cl = colorb;

    return std::tuple(inter, normal, cl);
}
};