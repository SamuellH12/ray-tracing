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
    point pos (0, 0, 0);
    point mira (1, 0, 0);
    auto x = pos - mira;
    vetor up (0, 1, 0);
    double dist = 10;
    int v_res=600;
    int h_res=800; 

    Camera cam(pos, mira, up, dist, v_res, h_res);

    std::vector<objeto*> objs;
    esfera* a = new esfera(point(45, -45, 45), 20, vetor(0, 0, 1));
    objs.push_back(a);
   
    std::vector<vetor> cores = cam.shot(objs);

    cout << "P3\n" << h_res << " " << v_res << "\n255\n";
    for(auto &v : cores)
        cout << (int)(v.getX()*255) << " " << (int)(v.getY()*255) << " " << (int)(v.getZ()*255) << endl;

    return 0;
}