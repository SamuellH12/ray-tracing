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

    bool has_intersection(ray &r, double tmax = std::numeric_limits<double>::infinity()) override{
        double n = normal * r.get_direction();

        if(n == 0.0) return false;

        double d = normal * (pos - r.get_origin());
        double t = (d/n);

        return t >= 0.0 && t <= tmax;
    }

    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos) override{ 
        double n = normal * r.get_direction();

        if(n == 0.0) return Intersection();

        double d = normal * (pos - r.get_origin());
        double t = (d/n);

        Color cl = get_color(r, r.get_point(t), normal, Ia, luzes, objetos);

        return t >= 0.0 ? Intersection(t, normal, cl) : Intersection();
    } // point inter = r.get_origin() + r.get_direction()*t;
};