#include <iostream>
#include "src/ObjReader.cpp"
#include "src/Camera.cpp"
#include "src/Vector.cpp"
// #include "src/Objetos/Objeto.cpp"
#include "src/Objetos/Esfera.cpp"
#include "src/Objetos/Plano.cpp"
#include <vector>

/*

Divirtam-se :)

*/

int main(){

    // objReader obj("inputs/cubo.obj");
    // obj.print_faces();
    point pos (-5, -10, 0);
    point mira (0, 0, 0);
    vetor up (0, 1, 0);
    double dist = 1;
    int v_res=1080;
    int h_res=1920; 

    Camera cam(pos, mira, up, dist, v_res, h_res);

    std::vector<objeto*> objs;
    // objs.emplace_back(new esfera(point(20, 0, 50), 400, vetor(1, 1, 0)));
    // objs.emplace_back(new esfera(point(700, 700, 1135), 340, vetor(1, 0, 1)));
    objs.emplace_back(new esfera(point(400, 0, 0), 250, vetor(0, 1, 1)));
    objs.emplace_back(new esfera(point(0, 0, 0), 100, vetor(0, 0, 0)));
    objs.emplace_back(new esfera(point(-100, 0, 0), 50, vetor(1, 0, 0)));
    objs.emplace_back(new esfera(point(0, 0, 0), 50, vetor(1, 0, 1)));
    // objs.emplace_back(new esfera(point(100, 0, 0), 200, vetor(0.6, 0.6, 0.8)));
    objs.emplace_back(new plano(point(0, -2000, 0), vetor(0, 1, 0), vetor(0.3, 0.8, 0.4)));

    for(int i=0; i<20; i++)
        objs.emplace_back(new esfera(point(500, 0, 500*i), 100, vetor(0.8, 0.2, 0.2)));

    std::vector<vetor> cores = cam.shot(objs);

    cout << "P3\n" << h_res << " " << v_res << "\n255\n";
    for(auto &v : cores)
        cout << (int)(v.getX()*255) << " " << (int)(v.getY()*255) << " " << (int)(v.getZ()*255) << endl;
    
    return 0;
}