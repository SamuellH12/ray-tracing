#include <iostream>
#include <fstream>
#include "src/ObjReader.cpp"
#include "src/Camera.cpp"
#include "src/Vector.cpp"
#include "src/Objetos/Esfera.cpp"
#include "src/Objetos/Plano.cpp"
#include "src/Objetos/Tabuleiro.cpp"
#include <vector>


int main(){
    // objReader obj("inputs/cubo.obj");
    // obj.print_faces();
    ofstream Saida("out.ppm", ios::trunc);
    point pos (1500, 800, -3500);
    // point mira(300, 300, 0); 
    point mira(300, 300, 300); 
    vetor up (-0.5, 1, 0);
    double dist = 0.75;
    int v_res=1080; 1920;
    int h_res= 1920;

    Camera cam(pos, mira, up, dist, v_res, h_res);

    std::vector<objeto*> objs;
    objs.emplace_back(new esfera(point(10000, -2000, -8000), 1500, vetor(1, 1, 0)));
    objs.emplace_back(new esfera(mira, 50, vetor(1, 0, 0)));
    objs.emplace_back(new esfera(point(1000, 0, 2000), 500, vetor(1, 0, 0)));
    objs.emplace_back(new tabuleiro(point(0, -75, 0), vetor(0, 1, 0), vetor(100, 0, 0), vetor(0.3, 0.8, 0.4), vetor(1, 1, 1)));


    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                objs.emplace_back(new esfera(point(i*300, j*300, k*300), 75 , vetor(0.5*i, 0.5*j, 0.5*k)));

    std::vector<vetor> cores = cam.shot(objs);

    Saida << "P3\n" << h_res << " " << v_res << "\n255\n";
    for(auto &v : cores)
        Saida << (int)(v.getX()*255) << " " << (int)(v.getY()*255) << " " << (int)(v.getZ()*255) << endl;
    
    Saida.close();

    for(objeto* obj: objs)
        delete obj;
    objs.clear();

    string command = "py convert_ppm.py";
    system(command.c_str());

    return 0;
}