#ifndef OBJETOHEADER
#define OBJETOHEADER
#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include <vector>
#include <tuple>
#include "../Luz.cpp"

struct Intersection {
    double dist = 1.0/0.0; //double inf
    Color color;
    vetor normal;
    int metadata = -1;

    Intersection(){}
    Intersection(double dist, vetor normal, Color color) : dist(dist), color(color), normal(normal){}
    Intersection(double dist, vetor normal, Color color, int meta) : dist(dist), color(color), normal(normal), metadata(meta){}
    Intersection(Color color) : color(color) {}

    // get_reflection(vetor direction)
    // get_refraction(vetor direction)
    point get_point(ray &r){ return r.get_origin() + r.get_direction() * dist; }

    bool operator< (const Intersection &b) const { return dist < b.dist; }
};

class objeto {
private:
protected:
    point pos;
    
    Color kd; // difuso
    Color ks; // specular
    Color ka; // ambiental
    Color ke; // Emissive
    Color kt; // transmissão // double ni; //Optical density // double d; // Kt=(1−d) ⋅ 4/(Ni+1)^2
    double ns = 1; // mi // potencia // Shininess 
    double ni = 1;
    double d = 1;
    // Color kr; // reflexao

public:
    objeto(){}
    objeto(point pos) : pos(pos) {}
    objeto(point pos, Color cor) : pos(pos), kd(cor) {}
    objeto(point pos, Color kd, Color ks, Color ka, Color ke, Color kt, double ns) : 
    pos(pos), kd(kd), ks(ks), ka(ka), ke(ke), kt(kt), ns(ns) {}

    objeto(point pos, Color kd, Color ks, Color ka, Color ke, double ni, double d, double ns) : 
    pos(pos), kd(kd), ks(ks), ka(ka), ke(ke), kt(kt), ns(ns), ni(ni), d(d) 
    {
        double t = (1.0-d) * (4.0 / ((ni+1)*(ni+1)) );
        kt = Color(t, t, t);
    }

    virtual bool has_intersection(ray &r, double tmax = std::numeric_limits<double>::infinity()){ return false; }

    virtual Intersection get_intersection(ray &r, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos){ return Intersection(); }
    
    virtual Color get_color(ray &r, point p, vetor normal, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos){ 
        Color I = ka^Ia.I;
        point pt = p + (r.get_direction() * -1 * (1e-6));
        for(auto [Pi, Ii] : luzes)
        {
            vetor Li = Pi - pt;
            double tmax = Li.norm();
            Li = Li.normalized();
            ray q (pt, Li);
            vetor Ri = normal*2.0*(normal*Li) - Li;

            bool ok = true;
            for(auto obj : objetos)
                if(obj->has_intersection(q, tmax)){
                    ok = false;
                    break;
                }

            if(!ok) continue;
            vetor v = r.get_direction()*-1;

            I = I + ((Ii ^ kd) * (normal*Li)) + ((Ii ^ ks) * pow(Ri*v, ns));
        }
        return I;
    }

    point get_pos(){ return pos; }
    void  set_pos(point p){ this->pos = p; }

    void setka(Color k){ ka = k; }
    void setkd(Color k){ kd = k; }
    void setks(Color k){ ks = k; }
};

#endif

/*

- Coeficiente difuso $\small \longrightarrow {k_d} \in [0, 1]^3$
- Coeficiente especular $\small \longrightarrow {k_s} \in [0, 1]^3$
- Coeficiente ambiental $\small \longrightarrow {k_a} \in [0, 1]^3$
- Coeficiente de reflexão $\small \longrightarrow {k_r} \in [0, 1]^3$
- Coeficiente de transmissão $\small \longrightarrow {k_t} \in [0, 1]^3$
- Coeficiente de rugosidade $\small \longrightarrow \eta > 0$



*/
