#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include <vector>
#include <tuple>
#pragma once

struct Intersection {
    double dist = 1.0/0.0; //double inf
    Color color;
    vetor normal;

    Intersection(){}
    Intersection(double dist, vetor normal, Color color) : dist(dist), color(color), normal(normal){}
    Intersection(Color color) : color(color) {}

    // get_reflection(vetor direction)
    // get_refraction(vetor direction)
    // get_point(ray r) return r.origin + r.dir * dist

    bool operator< (const Intersection &b) const { return dist < b.dist; }
};

class objeto {
private:
protected:
    point pos;
    vetor color;

public:
    objeto(){}
    objeto(point pos, vetor color) : pos(pos), color(color) {}

    virtual Intersection get_intersection(ray &r){ return Intersection(); }
};
