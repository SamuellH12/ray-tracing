#include <iostream>
#include <fstream>
#include "src/ObjReader.cpp"
#include "src/Camera.cpp"
#include "src/Vector.cpp"
#include "src/Objetos/Esfera.cpp"
#include "src/Objetos/Plano.cpp"
#include "src/Objetos/Tabuleiro.cpp"
#include "src/Objetos/Malha.cpp"
#include <vector>
point pos (0, 5, 15);
point mira(0, 0, 0); 
vetor up (0, 1, 0);
double dist = 0.75;
const int V_RES=600;
const int H_RES= 800;

Camera cam(pos, mira, up, dist, H_RES, V_RES);
std::vector<objeto*> objs;

void generate_img(){
    ofstream Saida("out.ppm", ios::trunc);
    std::vector<vetor> cores = cam.shot(objs);

    Saida << "P3\n" << H_RES << " " << V_RES << "\n255\n";
    for(auto &v : cores)
        Saida << min((int)(v.getX()*255), 255) << " " << min((int)(v.getY()*255), 255) << " " << min((int)(v.getZ()*255), 255) << endl;
    
    Saida.close();

    string command = "py convert_ppm.py";
    system(command.c_str());
}

const double PI = acos(-1);

int main(){
    std::cout << std::fixed << std::setprecision(4);

    // matrizes de operadores afins
    matrix<4, 4> m ({
        {2, 0, 0, 2},
        {0, 1, 0, 2},
        {0, 0, 3, 2},
        {0, 0, 0, 1},
    });
    matrix<4, 4> translacao ({
        {1, 0, 0, 0},
        {0, 1, 0, -10},
        {0, 0, 1, 0},
        {0, 0, 0,  1},
    });
    auto r = MatrixZRotation(PI/4);
    
    matrix<4, 4> escala ({
        {10, 0, 0, 0},
        {0, 10, 0, 0},
        {0, 0, 10, 0},
        {0, 0, 0,  1},
    });

    /*************************************/
    // Adicionar objetos na cena
    // objs.emplace_back(new tabuleiro(point(0, -7, 0), vetor(0, 1, 0), vetor(1, 0, 0), vetor(0.3, 0.8, 0.4), vetor(1, 1, 1)));
    
    objReader obj("inputs/cubo.obj");
    objReader obj2("inputs/cubo2.obj");
    auto cubo = new malha(obj);
    auto cubo2 = new malha(obj2);
    objs.emplace_back( cubo );
    objs.emplace_back( cubo2 );
    
    objReader objm("inputs/monkey.obj");
    auto macaco = new malha(objm);
    objs.emplace_back( macaco );
    macaco->affine_transform(m);


    /*************************************/
    // Capturar imagem

    cubo2->affine_transform(translacao);
    // cubo->affine_transform(m);
    // cubo->affine_transform(r);
    generate_img();
    
    
    // auto x = r * m;
    // cubo2->affine_transform(x);
    
    // generate_img();

    
    // deletar objetos?
    for(objeto* obj: objs) delete obj;
    objs.clear();
    return 0;
}
