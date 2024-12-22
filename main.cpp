#include <iostream>
#include <fstream>
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
    ofstream Saida("out.ppm", ios::trunc);
    point pos (-2000, 1000, 500);
    point mira(300, 300, 0); 
    vetor up (0, 1, 0);
    double dist = 1.0;
    int v_res=1920;
    int h_res=1920;

    Camera cam(pos, mira, up, dist, v_res, h_res);

    std::vector<objeto*> objs;
    objs.emplace_back(new esfera(point(10000, -2000, -8000), 1500, vetor(1, 1, 0)));
    objs.emplace_back(new plano(point(0, -2000, 0), vetor(0, 1, 0), vetor(1, 1, 1)));

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                objs.emplace_back(new esfera(point(i*300, j*300, k*300), 75 , vetor(0.5*i, 0.5*j, 0.5*k)));

    std::vector<vetor> cores = cam.shot(objs);

    Saida << "P3\n" << h_res << " " << v_res << "\n255\n";
    for(auto &v : cores)
        Saida << (int)(v.getX()*255) << " " << (int)(v.getY()*255) << " " << (int)(v.getZ()*255) << endl;
    
    Saida.close();
    return 0;
}