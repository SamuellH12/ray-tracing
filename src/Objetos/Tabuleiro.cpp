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
Color colora, colorb;
long long tsize = -1;

public:
    tabuleiro(){}
    tabuleiro(point pos, vetor normal, vetor dir, Color color1, Color color2, long long tsize = -1) : objeto(pos, color1), normal(normal), dir(dir), colora(color1), colorb(color2), tsize(tsize){
        dir = dir - (normal * ((normal*dir) / (normal*normal)));
        ort = (normal % dir).normalized() * sqrt(dir*dir);
        ks = Color(1, 1, 1) * 0.01;
    }

    double dist_intersection(ray &r) override{
        double n = normal * r.get_direction();

        if(n == 0.0) return DOUBLEINF;

        double d = normal * (pos - r.get_origin());
        double t = (d/n);

        // if(t <= 0) return DOUBLEINF;

        point inter = r.get_origin() + r.get_direction()*t;
        vetor oi = inter - pos;  //origem -> interseção
        int x = floor((oi*dir) / (dir*dir)); //calcula a projeção do vetor oi na direção
        int y = floor((oi*ort) / (ort*ort));
        
        if(tsize != -1 && (abs(x) > tsize || abs(y) > tsize)) return DOUBLEINF;

        return t;
    }

    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade = MAXREC) override{
        double n = normal * r.get_direction();

        if(n == 0.0) return Intersection();

        double d = normal * (pos - r.get_origin());
        double t = (d/n);

        if(t < 0.0) return Intersection();

        point inter = r.get_point(t);
        vetor oi = inter - pos;  //origem -> interseção
        int x = floorl((oi*dir) / (dir*dir)); //calcula a projeção do vetor oi na direção
        int y = floorl((oi*ort) / (ort*ort));
        
        if(tsize != -1 && (abs(x) > tsize || abs(y) > tsize)) return Intersection();
        
        ka = kd = (abs(x)%2 == abs(y)%2) ?  colorb : colora;

        Color cl = get_color(r, inter, normal, Ia, luzes, objetos, profundidade);
        return Intersection(t, normal, cl);
    }
};
#endif