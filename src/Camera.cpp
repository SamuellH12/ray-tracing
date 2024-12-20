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
        w = (u % v).normalized();
    }

    //retorna um vetor com as cores
    std::vector<vetor> shot(std::vector<objeto*> objetos){
        std::vector<vetor> tela;

        point canto_up_lft = c + u*(v_res/2.0 - 0.5) - w*(h_res/2.0 - 0.5) - v*dist;

        for(int x=0; x<v_res; x++) {
            for(int y=0; y<h_res; y++)
            {
                point o = canto_up_lft + w*y - u*x;
                ray r (c, (o - c));

                vetor color(0, 0, 0);
                double dist = 1.0/0.0; //double INF

                for(auto &obj : objetos){
                    if(!obj->has_intersection(r)) continue;
                    auto [inter, normal, col] = obj->get_intersection(r);

                    auto d = (inter - o) * (inter - o);
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