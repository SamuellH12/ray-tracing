#include <iostream>
#include <fstream>
#include "src/ObjReader.cpp"
#include "src/Camera.cpp"
#include "src/Vector.cpp"
#include "src/Luz.cpp"
#include "src/Objetos/Esfera.cpp"
#include "src/Objetos/Plano.cpp"
#include "src/Objetos/Tabuleiro.cpp"
#include "src/Objetos/Malha.cpp"
#include "scenes/sceneObj.cpp"
#include "scenes/templateScene.cpp"
#include <vector>
#pragma GCC optimize("O3")

int minmax(int x){ return max(min(x, 255), 0); }

void generate_img(Scene cena){
    ofstream Saida("out.ppm", ios::trunc);
    std::vector<vetor> cores = cena.shot();

    Saida << "P3\n" << cena.h_res << " " << cena.v_res << "\n255\n";
    for(auto &v : cores)
        Saida << minmax(v.getX()*255) << " " << minmax(v.getY()*255) << " " << minmax(v.getZ()*255) << endl;
    
    Saida.close();

    string command = "py convert_ppm.py";
    system(command.c_str());
    
    cout << "Ok ok! =]\n";
}


int main(){
    std::cout << std::fixed << std::setprecision(4);

    // Scene cena_sample = get_scene_sample();
    // generate_img(cena_sample);
    
    // Scene cena_debug = get_scene_debug();
    // generate_img(cena_debug);

    Scene cena_classica = get_scene_classic();
    cerr << "ok?";
    ray r(cena_classica.cam.get_pos(), point(0, 3, 0));
    cena_classica.objetos.front()->get_intersection(r, cena_classica.La, cena_classica.luzes, cena_classica.objetos);
    generate_img(cena_classica);

    return 0;
}