#include <iostream>
#include "src/ObjReader.cpp"
#include "src/Camera.cpp"
#include "src/Vector.cpp"
#include "src/Objetos/Objeto.cpp"
#include "src/Objetos/Esfera.cpp"
#include "src/Objetos/Plano.cpp"
#include "src/Objetos/Tabuleiro.cpp"
#include <vector>

int main(){

    // objReader obj("inputs/cubo.obj");
    // obj.print_faces();
    vetor mover(-100, 6, 40); //mover = mover*40;
    point pos (-1600, -50, 1000);// pos = pos + mover;
    point mira (300, 300, 0); //mira = mira + mover;
    vetor up (0, 1, 0);
    double dist = 200;
    int v_res=1080;
    int h_res=1920;

    vetor v = (pos - mira) / 2.0;
    pos = pos + v;
    pos = pos + vetor(0, 800, 0);    

    Camera cam(pos, mira, up, dist, v_res, h_res);

    std::vector<objeto*> objs;
    // objs.emplace_back(new esfera(point(20, 0, 50), 400, vetor(1, 1, 0)));
    // objs.emplace_back(new esfera(point(700, 700, 1135), 340, vetor(1, 0, 1)));
    // objs.emplace_back(new esfera(point(400, 0, 0), 250, vetor(0, 1, 1)));
    // objs.emplace_back(new esfera(point(0, 0, 0), 100, vetor(0, 0, 0)));
    // objs.emplace_back(new esfera(point(-100, 0, 0), 50, vetor(1, 0, 0)));
    // objs.emplace_back(new esfera(point(100, 0, 0), 200, vetor(0.6, 0.6, 0.8)));
    // objs.emplace_back(new plano(point(0, -2000, 0), vetor(0, 1, 0), vetor(0.3, 0.8, 0.4)));
    objs.emplace_back(new esfera(point(10000, -2000, -8000), 1500, vetor(1, 1, 0)));
    objs.emplace_back(new tabuleiro(point(0, -2000, 0), vetor(0, 1, 0), vetor(800, 0, 0), vetor(0.3, 0.8, 0.4), vetor(1, 1, 1)));


    vetor repos(0, 0, 0);
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                objs.emplace_back(new esfera(point(i*300, j*300, k*300) + repos, 75 , vetor(0.5*i, 0.5*j, 0.5*k)));

    std::vector<vetor> cores = cam.shot(objs);

    cout << "P3\n" << h_res << " " << v_res << "\n255\n";
    for(auto &v : cores)
        cout << (int)(v.getX()*255) << " " << (int)(v.getY()*255) << " " << (int)(v.getZ()*255) << endl;
    
    return 0;
}