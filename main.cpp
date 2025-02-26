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
const int v_res=1080;
const int h_res= 1920;

Camera cam(pos, mira, up, dist, v_res, h_res);
std::vector<objeto*> objs;

void generate_img(){
    ofstream Saida("out.ppm", ios::trunc);
    std::vector<vetor> cores = cam.shot(objs);

    Saida << "P3\n" << h_res << " " << v_res << "\n255\n";
    for(auto &v : cores)
        Saida << (int)(v.getX()*255) << " " << (int)(v.getY()*255) << " " << (int)(v.getZ()*255) << endl;
    
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
    objs.emplace_back(new tabuleiro(point(0, -7, 0), vetor(0, 1, 0), vetor(1, 0, 0), vetor(0.3, 0.8, 0.4), vetor(1, 1, 1)));
    
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


    // cubo->get_centroid().print();
    // ray raio (pos, cubo->get_centroid() - pos);
    // std::cout << cubo->get_intersection(raio).dist << "\n";

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

/*** OBJETOS DE TESTE
    objs.emplace_back(new esfera(point(10000, -2000, -8000), 1500, vetor(1, 1, 0)));
    objs.emplace_back(new esfera(mira, 50, vetor(1, 0, 0)));
    objs.emplace_back(new esfera(point(1000, 0, 2000), 500, vetor(1, 0, 0)));
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                objs.emplace_back(new esfera(point(i*300, j*300, k*300), 75 , vetor(0.5*i, 0.5*j, 0.5*k)));
*/