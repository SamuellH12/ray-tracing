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
    plano(point pos, vetor normal, Color color) : objeto(pos, color), normal(normal) {}

    Intersection get_intersection(ray &r) override{ 
        double n = normal * r.get_direction();

        if(n == 0.0) return Intersection();

        double d = normal * (pos - r.get_origin());
        double t = (d/n);

        return t >= 0.0 ? Intersection(t, normal, color) : Intersection();
    } // point inter = r.get_origin() + r.get_direction()*t;
};