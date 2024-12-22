#ifndef CAMERAHEADER
// #define CAMERAHEADER
#include <iostream>
#include <math.h>
#include "Point.cpp"
#include "Vector.cpp"
#include "Ray.cpp"
#include "Objetos/Objeto.cpp"

/*
Classe de vetores.
A classe precisa ser instanciada passando as componentes x, y e z
*/
/*
Operações a implementar:
ok Soma de vetores -> vetor
ok subtração de vetores -> vetor
ok mult por escalar -> vetor
ok div por escalar -> vetor
ok produto interno -> escalar
Get perpendicular to vector
get perpendicular to plane
*/


class Camera{
    private:
        point c; //position
        point m; //mira
        vetor up; //cima
        vetor u, v, w; // vetores ortonormais da base da camera
        int v_res = 600; //vertical resolturion
        int h_res = 800; //horizontal resolution / valor padrão arbitrário
        double dist;
        point canto_up_lft;
    
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
        w = (u % v).normalized() * -1;
        canto_up_lft = m - w*(h_res/2.0 - 0.5) - v*dist + u*(v_res/2.0 - 0.5);
        // u.print();
        // v.print()        ;
        // w.print();
    }


    //retorna um vetor com as cores
    std::vector<vetor> shot(std::vector<objeto*> &objetos, vetor backgroud_top = vetor(0, 0.3, 0.7), vetor backgroud_bottom = vetor(1, 1, 1)){
        
        std::vector<vetor> tela;
        double t;
        for(int y=0; y<v_res; y++) {
            for(int x=0; x<h_res; x++) {
                point o = canto_up_lft + w*x - u*y;
                ray r (c, (o-c).normalized());

                vetor color = backgroud_top * ((double)(v_res - y)/(double)v_res) + backgroud_bottom * ((double)y/(double)v_res);
                double dist = 1.0/0.0; //double INF

                for(auto &obj : objetos){
                    if(!obj->has_intersection(r)) continue;
                    auto [inter, normal, col] = obj->get_intersection(r);

                    auto d = (inter - c) * (inter - c);
                    if(d < dist) {
                        dist = d;
                        color = col;
                    }
                }

                tela.push_back(color);
            }
        }
        return tela;
    }
};

#endif