#ifndef CAMERAHEADER
#define CAMERAHEADER
#include <iostream>
#include <math.h>
#include "Point.cpp"
#include "Vector.cpp"
#include "Ray.cpp"
#include "Objetos/Objeto.cpp"

class Camera{
    private:
        point c; //position
        point m; //mira
        vetor up; //cima
        vetor u, v, w; // vetores ortonormais da base da camera
        int v_res = 600; //vertical resolturion
        int h_res = 800; //horizontal resolution / valor padrão arbitrário
        double dist;
    
    public:

    //Construtores
    Camera() {}
    Camera(point pos, point mira, vetor up, double dist, int v_res=600, int h_res=800) 
    : c(pos), m(mira), up(up), dist(dist), v_res(v_res), h_res(h_res)
    {
        v = (m - c).normalized() * -1.0;
        up = up.normalized();
        u = up - ( v * ((v*up) / (v*v)));
        u = u.normalized();
        w = (v % u).normalized();
    }

    //retorna um vetor com as cores
    std::vector<Color> shot(std::vector<objeto*> const &objetos, vetor backgroud_top = vetor(0.3, 0.3, 0.7), vetor backgroud_bottom = vetor(1, 1, 1)){
        std::vector<Color> tela;

        double pixel_unit = 1.0 / h_res;

        for(int y=0; y<v_res; y++) {
            for(int x=0; x<h_res; x++)
            {
                // double px = ((2.0 * x + 1.0)/ (h_res) - 1);
                // double py = ((2.0 * y + 1.0)/ (v_res) - 1);
                // vetor d =  w * px - u * py - v*dist;
                
                point tl = c - v*dist   +  u*(pixel_unit*(v_res/2.0 - y - 0.5))  -  w*(pixel_unit*(h_res/2.0 - x - 0.5));
                ray r(c, (tl-c));

                Intersection inter (backgroud_top * ((double)(v_res - y)/(double)v_res) + backgroud_bottom * ((double)y/(double)v_res));
                
                for(auto &obj : objetos)
                    inter = min(inter, obj->get_intersection(r));

                tela.push_back(inter.color);
            }
        }

        return tela;
    }
};

#endif