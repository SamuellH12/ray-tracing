#ifndef CILINDROHEADER
#define CILINDROHEADER
#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "Objeto.cpp"
#include <vector>
#include <tuple>

class cilindro : public objeto {
private:
    double h;
    vetor v;
    bool oco;

public:
    cilindro(){}
    cilindro(point pos, vetor v, double raio, Color color, bool preenchido=true) : objeto(pos, color), v(v), h(raio), oco(!preenchido) {}
    cilindro(point pos, point b, double raio, Color color, bool preenchido=true) : objeto(pos, color), v(b-pos), h(raio), oco(!preenchido) {}

    Intersection disk_inter(ray &r, point center){
        vetor normal = v.normalized();
        double denom = normal * r.get_direction();
        if (fabs(denom) < 1e-6) return Intersection(); // paralelo

        vetor oc = center - r.get_origin();

        double t = (oc * normal) / denom;
        if (t < 0) return Intersection();

        point inter = r.get_origin() + r.get_direction() * t;

        if ((inter - center).norm2() > h*h) return Intersection();
        return Intersection(t, normal, ka);
    }

    bool disk_has_inter(ray &r, point center, double tmax = std::numeric_limits<double>::infinity()){
        vetor normal = v.normalized();
        double denom = normal * r.get_direction();
        if (fabs(denom) < 1e-6) return false; // paralelo

        vetor oc = center - r.get_origin();

        double t = (oc * normal) / denom;
        if (t < 0) return false;

        point inter = r.get_origin() + r.get_direction() * t;

        if ((inter - center).norm2() > h*h) return false;
        return t < tmax;
    }

    bool has_intersection(ray &r, double tmax = std::numeric_limits<double>::infinity()) override{
        if(disk_has_inter(r, pos, tmax) || disk_has_inter(r, pos+v, tmax)) return true;
        vetor oc = r.get_origin() - pos;
        vetor dir = r.get_direction();
        vetor axis_norm = v.normalized();

        double a = (dir - axis_norm * (dir * axis_norm)).norm2();
        double b = 2.0 * ((dir - axis_norm * (dir * axis_norm)) * (oc - axis_norm * (oc * axis_norm)));
        double c = (oc - axis_norm * (oc * axis_norm)).norm2() - h*h;
        double dlt = b*b - 4.0*a*c;

        if (dlt < 0.0) return false;

        double raiz = sqrt(dlt);
        double t = (-b - raiz) / (2.0 * a);
        bool changed = false;
        if (t < 0) t = (-b + raiz) / (2.0 * a), changed = true;
        if (t < 0) return false;

        point inter = r.get_origin() + r.get_direction() * t;
        double projection = (inter - pos) * axis_norm;

        if(!changed && (projection < 0 || projection*projection > v.norm2())) // se falhou, testa com o outro
        { 
            t = (-b + raiz) / (2.0 * a);
            inter = r.get_origin() + r.get_direction() * t;
            projection = (inter - pos) * axis_norm;
        }

        if (projection < 0 || projection*projection > v.norm2()) return false;

        return t < tmax;
    }

    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade = MAXREC) override{ 
        auto setintercolor = [&](Intersection &inter) -> Intersection& {
            if(inter.dist <= (1.0/0.0)) inter.color = get_color(r, r.get_point(inter.dist), inter.normal, Ia, luzes, objetos, profundidade);
            return inter;
        };
        
        auto interfinal = std::min(disk_inter(r, pos), disk_inter(r, pos+v));
        vetor oc = r.get_origin() - pos;
        vetor dir = r.get_direction();
        vetor axis_norm = v.normalized();

        double a = (dir - axis_norm * (dir * axis_norm)).norm2();
        double b = 2.0 * ((dir - axis_norm * (dir * axis_norm)) * (oc - axis_norm * (oc * axis_norm)));
        double c = (oc - axis_norm * (oc * axis_norm)).norm2() - h*h;
        double dlt = b*b - 4.0*a*c;

        if (dlt < 0.0) return setintercolor(interfinal);

        double raiz = sqrt(dlt);
        double t = (-b - raiz) / (2.0 * a);
        bool changed = false;
        if (t < 0) t = (-b + raiz) / (2.0 * a), changed = true;
        if (t < 0) return setintercolor(interfinal); // std::min(disk_inter(r, pos), disk_inter(r, pos+v));

        point inter = r.get_origin() + r.get_direction() * t;
        double projection = (inter - pos) * axis_norm;
        
        if(!changed && (projection < 0 || projection*projection > v.norm2())) // se falhou, testa com o outro
        { 
            t = (-b + raiz) / (2.0 * a);
            inter = r.get_origin() + r.get_direction() * t;
            projection = (inter - pos) * axis_norm;
        }

        if (projection < 0 || projection*projection > v.norm2()) return setintercolor(interfinal); // : std::min(disk_inter(r, pos), disk_inter(r, pos+v));

        vetor normal = inter - (pos + axis_norm * projection);
        double seno = (normal%r.get_direction()).norm() / (normal.norm() * (r.get_direction().norm()));

        interfinal =  std::min(Intersection(t, normal, ka), interfinal); 
        return setintercolor(interfinal);
    }
};
#endif