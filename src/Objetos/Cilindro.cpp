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

    double disk_dist_inter(ray &r, point center){
        vetor normal = v.normalized();
        double denom = normal * r.get_direction();
        if (fabs(denom) < 1e-6) return DOUBLEINF; // paralelo

        vetor oc = center - r.get_origin();

        double t = (oc * normal) / denom;
        if (t < 0) return DOUBLEINF;

        point inter = r.get_origin() + r.get_direction() * t;

        if ((inter - center).norm2() > h*h) return DOUBLEINF;
        return t;
    }

    double dist_intersection(ray &r) override{
        double t1 = std::min(disk_dist_inter(r, pos), disk_dist_inter(r, pos+v));

        vetor oc = r.get_origin() - pos;
        vetor dir = r.get_direction();
        vetor axis_norm = v.normalized();

        double a = (dir - axis_norm * (dir * axis_norm)).norm2();
        double b = 2.0 * ((dir - axis_norm * (dir * axis_norm)) * (oc - axis_norm * (oc * axis_norm)));
        double c = (oc - axis_norm * (oc * axis_norm)).norm2() - h*h;
        double dlt = b*b - 4.0*a*c;

        if (dlt < 0.0) return t1;

        double raiz = sqrt(dlt);
        double t = (-b - raiz) / (2.0 * a);
        bool changed = false;
        if (t < 0) t = (-b + raiz) / (2.0 * a), changed = true;
        if (t < 0) return t1;

        point inter = r.get_origin() + r.get_direction() * t;
        double projection = (inter - pos) * axis_norm;

        if(!changed && (projection < 0 || projection*projection > v.norm2())) // se falhou, testa com o outro
        { 
            t = (-b + raiz) / (2.0 * a);
            inter = r.get_origin() + r.get_direction() * t;
            projection = (inter - pos) * axis_norm;
        }

        if (projection < 0 || projection*projection > v.norm2()) return t1;

        return std::min(t, t1);
    }

    Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade = MAXREC) override{ 
        auto setintercolor = [&](Intersection &inter) -> Intersection& {
            // if(inter.dist <= (1.0/0.0)) inter.color = get_color(r, r.get_point(inter.dist), inter.normal, Ia, luzes, objetos, profundidade-1);
            inter.color = ka;
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