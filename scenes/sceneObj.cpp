#ifndef SCENEOBJHEADER
#define SCENEOBJHEADER
#include "../src/ObjReader.cpp"
#include "../src/Camera.cpp"
#include "../src/Vector.cpp"
#include "../src/Luz.cpp"
#include "../src/Objetos/Esfera.cpp"
#include "../src/Objetos/Plano.cpp"
#include "../src/Objetos/Tabuleiro.cpp"
#include "../src/Objetos/Malha.cpp"
#include <vector>

struct Scene {
    Camera cam;
    std::vector<objeto*> objetos;
    std::vector<Luz> luzes;
    Luz La;
    int h_res, v_res;

    Scene(Camera cam, Luz La = Color(0.5, 0.5, 0.5)) : cam(cam), La(La) {
        h_res = cam.h_res;
        v_res = cam.v_res;
    }
    void add_obj(objeto* o){ objetos.emplace_back(o); }
    void add_luz(Luz l){ luzes.emplace_back(l); }

    std::vector<Color> shot(){ return cam.shot(objetos, luzes, La); }

    void delete_objs(){
        for(objeto* obj: objetos) delete obj;
        objetos.clear();
    }
};

#endif