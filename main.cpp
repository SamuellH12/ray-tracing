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
    point mira (0, 0, 1);
    auto x = pos - mira;
    vetor up (0, 1, 0);
    double dist = 1;
    int v_res=600;
    int h_res=800; 

    Camera cam(pos, mira, up, dist, v_res, h_res);

    std::vector<objeto*> objs;
    esfera* a = new esfera(point(0, 0, 5), 2, vetor(1, 1, 0));
    esfera* b = new esfera(point(2, 0, 5), 1, vetor(1, 0, 1));
    esfera* c = new esfera(point(-2, 0, 5), 1.3, vetor(0, 1, 1));
    plano* p = new plano(point(0, 0, 0), vetor(1, 0, 0), vetor(1, 1, 0));

    objs.push_back(p);
    objs.push_back(a);
    objs.push_back(b);
    objs.push_back(c);
   
    std::vector<vetor> cores = cam.shot(objs);

    cout << "P3\n" << h_res << " " << v_res << "\n255\n";
    for(auto &v : cores)
        cout << (int)(v.getX()*255) << " " << (int)(v.getY()*255) << " " << (int)(v.getZ()*255) << endl;

    delete a;
    delete b;
    delete c;
    delete p;
    
    return 0;
}