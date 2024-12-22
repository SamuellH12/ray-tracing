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

virtual bool has_intersection(ray r, double &t){ 
    vetor D = r.get_direction();
    point O = r.get_origin();

    vetor L = pos - O; 
    double d = normal * L;
    double n = normal * D;

    if(n == 0.0) return false;
    
    t = (d/n);
    return t > 0;
}
};