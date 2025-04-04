#ifndef CAMERAHEADER
#define CAMERAHEADER
#include <iostream>
#include <math.h>
#include "Point.cpp"
#include "Vector.cpp"
#include "Ray.cpp"
#include "Objetos/Objeto.cpp"
#include "Objetos/Esfera.cpp"

class Camera{
    private:
        point c; //position
        point m; //mira
        vetor up; //cima
        vetor u, v, w; // vetores ortonormais da base da camera
        double dist;
    
    public:
        int v_res = 600; //vertical resolturion
        int h_res = 800; //horizontal resolution / valor padrão arbitrário

    //Construtores
    Camera() {}
    Camera(point pos, point mira, vetor up, double dist, int v_res=800, int h_res=600) 
    : c(pos), m(mira), up(up), dist(dist), v_res(v_res), h_res(h_res)
    {
        v = (m - c).normalized() * -1.0;
        up = up.normalized();
        u = up - ( v * ((v*up) / (v*v)));
        u = u.normalized();
        w = (v % u).normalized();
    }

    //retorna um vetor com as cores
    std::vector<Color> shot(std::vector<objeto*> const &objetos, std::vector<Luz> const &luzes, Luz Ia = Luz(Color(0.25, 0.25, 0.25)), bool showpercentage = true, vetor backgroud_top = vetor(0.3, 0.3, 0.7), vetor backgroud_bottom = vetor(1, 1, 1)){
        std::vector<Color> tela(v_res*h_res);

        double pixel_unit = 1.0 / h_res;

        for(int y=0; y<v_res; y++) {
            #pragma omp parallel for
            for(int x=0; x<h_res; x++)
            {
                point tl = c - v*dist   +  u*(pixel_unit*(v_res/2.0 - y - 0.5))  -  w*(pixel_unit*(h_res/2.0 - x - 0.5));
                ray r(c, (tl-c));

                double seno = ( 1.0 + r.get_direction().getY() ) / 2.0; // calcula o ângulo pra usar como porcentagem da cor do ceu 
                __background = backgroud_top * ((double)(v_res - y)/(double)v_res) + backgroud_bottom * ((double)y/(double)v_res);
                Intersection inter(__background); 
                objeto* obi = NULL;
                double dist = DOUBLEINF;

                for(auto &obj : objetos){
                    auto dt = obj->dist_intersection(r);
                    if(dt < dist && dt > 0.0)
                        dist = dt,
                        obi = obj;
                }
                
                if(obi) inter = std::min<Intersection>(inter, obi->get_intersection(r, Ia, luzes, objetos));

                tela[x + y*h_res] = inter.color;
            }
            if(showpercentage) std::cout << (y+1.0) / v_res * 100.0 << "%\r";
        }

        return tela;
    }

    void move(vetor delta){
        c = c + delta;
        m = m + delta;
    }
    void set_position(point pos){
        m = m + (pos-c);
        c = pos;
    }
    void set_dist(double d){ dist = d; m = c + v * (-dist); }
    double get_dist() const { return dist; }

    point get_pos() const { return c; }
    vetor get_u() const { return u; } //y axis
    vetor get_v() const { return v; } //x axis
    vetor get_w() const { return w; } //z axis

    void rotatex(double theta) {
        u = u.rotatex(theta);
        v = v.rotatex(theta);
        w = w.rotatex(theta);
        m = c + v * (-dist);
    }
    void rotatey(double theta) {
        u = u.rotatey(theta);
        v = v.rotatey(theta);
        w = w.rotatey(theta);
        m = c + v * (-dist);
    }
    void rotatez(double theta) {
        u = u.rotatez(theta);
        v = v.rotatez(theta);
        w = w.rotatez(theta);
        m = c + v * (-dist);
    }

    void rotateu(double theta) {
        v = v.rotateAroundAxis(u, theta);
        w = w.rotateAroundAxis(u, theta);
        m = c + v * (-dist);
    }
    void rotatev(double theta) {
        u = u.rotateAroundAxis(v, theta);
        w = w.rotateAroundAxis(v, theta);
        m = c + v * (-dist);
    }
    void rotatew(double theta) {
        u = u.rotateAroundAxis(w, theta);
        v = v.rotateAroundAxis(w, theta);
        m = c + v * (-dist);
    }
};

#endif