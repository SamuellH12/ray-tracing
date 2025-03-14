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
    double ns = 4; // mi // potencia // Shininess 
    double ni = 1;
    double d = 1;
    // Color kr; // reflexao

public:
    objeto(){}
    objeto(point pos) : pos(pos) {}
    objeto(point pos, Color cor) : pos(pos), kd(cor), ka(cor*0.5) {}
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
        normal = normal.normalized();
        for(auto [Pi, Ii] : luzes)
        {
            vetor Li = Pi - pt;
            double tmax = Li.norm();
            Li = Li.normalized();
            ray q (pt, Li);
            vetor Ri = (normal*2.0*(normal*Li) - Li).normalized();

            bool ok = true;
            for(auto obj : objetos)
                if(obj->has_intersection(q, tmax)){
                    ok = false;
                    break;
                }

            if(!ok) continue;
            vetor v = r.get_direction()*-1;

            I = I + ((Ii ^ kd) * std::max(normal*Li, 0.0)) + ((Ii ^ ks) * pow(std::max(Ri*v, 0.0), ns));
        }
        return I;
    }

    point get_pos(){ return pos; }
    void  set_pos(point p){ this->pos = p; }

    void setka(Color k){  ka = k; }
    void setkd(Color k){  kd = k; }
    void setks(Color k){  ks = k; }
    void setns(double n){ ns = n; }
    void setni(double n){ ni = n; }
    void setd (double n){ d  = n; }
};


bool boundbox_intersection(ray &r, point bx1, point bx2){
    for(int __=0; __<2; __++, std::swap(bx1, bx2))
    {
        // plano X
        double n = r.get_direction().getX();
        if(abs(n) > 1e-6){
            double t = (bx1 - r.get_origin()).getX() / n;
            if(t > 0) {    
                point i = r.get_origin() + r.get_direction() * t;
                point&a = bx1;
                point c (a.getX(), bx2.getY(), bx2.getZ());
                point b (a.getX(), a.getY(), c.getZ());
                point d (a.getX(), c.getY(), a.getZ());

                int modulo = ((b-a) % (i - a)).getX() > 0 ? 1 : -1;
                if( ((c-b) % (i - b)).getX()*modulo > 0 
                &&  ((d-c) % (i - c)).getX()*modulo > 0 
                &&  ((a-d) % (i - d)).getX()*modulo > 0 )
                    return true;
            }
        }

        // plano Y
        n = r.get_direction().getY();
        if(abs(n) > 1e-6){
            double t = (bx1 - r.get_origin()).getY() / n;
            if(t > 0) {
                point i = r.get_origin() + r.get_direction() * t;
                point&a = bx1;
                point c (bx2.getX(), a.getY(), bx2.getZ());
                point b (a.getX(), a.getY(), c.getZ());
                point d (c.getX(), a.getY(), a.getZ());

                int modulo = ((b-a) % (i - a)).getY() > 0 ? 1 : -1;
                if( ((c-b) % (i - b)).getY()*modulo > 0 
                &&  ((d-c) % (i - c)).getY()*modulo > 0 
                &&  ((a-d) % (i - d)).getY()*modulo > 0 )
                    return true;
            }
        }

        // Plano Z
        n = r.get_direction().getZ();
        if(abs(n) > 1e-6){
            double t = (bx1 - r.get_origin()).getZ() / n;
            if(t > 0) {
                point i = r.get_origin() + r.get_direction() * t;
                point&a = bx1;
                point c (bx2.getX(), bx2.getY(), a.getZ());
                point b (c.getX(), a.getY(), a.getZ());
                point d (a.getX(), c.getY(), a.getZ());

                int modulo = ((b-a) % (i - a)).getZ() > 0 ? 1 : -1;
                if( ((c-b) % (i - b)).getZ()*modulo > 0 
                &&  ((d-c) % (i - c)).getZ()*modulo > 0 
                &&  ((a-d) % (i - d)).getZ()*modulo > 0 )
                    return true;
            }
        }

    }

    return false;
}

#endif
/*

- Coeficiente difuso $\small \longrightarrow {k_d} \in [0, 1]^3$
- Coeficiente especular $\small \longrightarrow {k_s} \in [0, 1]^3$
- Coeficiente ambiental $\small \longrightarrow {k_a} \in [0, 1]^3$
- Coeficiente de reflexão $\small \longrightarrow {k_r} \in [0, 1]^3$
- Coeficiente de transmissão $\small \longrightarrow {k_t} \in [0, 1]^3$
- Coeficiente de rugosidade $\small \longrightarrow \eta > 0$



*/
