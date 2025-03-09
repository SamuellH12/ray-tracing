#ifndef TABULEIROHEADER
#define TABULEIROHEADER
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
Color colorb;

public:
    tabuleiro(){}
    tabuleiro(point pos, vetor normal, vetor dir, Color color, Color colorb) : objeto(pos, color), normal(normal), dir(dir), colorb(colorb){
        dir = dir - (normal * ((normal*dir) / (normal*normal)));
        ort = (normal % dir).normalized() * sqrt(dir*dir);
    }

    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos) override{

        double n = normal * r.get_direction();

        if(n == 0.0) return Intersection();

        double d = normal * (pos - r.get_origin());
        double t = (d/n);

        if(t <= 0) return Intersection();

        point inter = r.get_origin() + r.get_direction()*t;
        vetor oi = inter - pos;  //origem -> interseção
        int x = floor((oi*dir) / (dir*dir)); //calcula a projeção do vetor oi na direção
        int y = floor((oi*ort) / (ort*ort));
        
        Color cl = (abs(x)%2 == abs(y)%2) ?  colorb : kd;

        return Intersection(t, normal, cl);
    }
};
#endif