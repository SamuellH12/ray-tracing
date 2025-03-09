#ifndef SCENEHEADER
#define SCENEHEADER
#include "../src/ObjReader.cpp"
#include "../src/Vector.cpp"
#include "../src/Luz.cpp"
#include "../src/Objetos/Esfera.cpp"
#include "../src/Objetos/Plano.cpp"
#include "../src/Objetos/Tabuleiro.cpp"
#include "../src/Objetos/Malha.cpp"
#include "sceneObj.cpp"
#include <vector>

Scene get_scene_debug(point camPos = point(10, 100, 10), point luzPos = point(10, 3, 10)){
    point mira(10, 0, 10); 
    vetor up (1.1, 0, 0);
    double dist = 0.75;
    const int v_res=1080/2;
    const int h_res= 1920/2;

    Scene cena(Camera(camPos, mira, up, dist, v_res, h_res), Luz(Color(0.01, 0.01, 0.01)));

    /*************************************/
    // Adicionar objetos na cena
    cena.luzes.emplace_back(Color(1, 1, 1), luzPos);

    cena.add_obj(new plano(point(0, -7, 0), vetor(0, 1, 0), Color(0.3, 0.8, 0.4)));
    cena.objetos.back()->setka( Color(0.3, 0.8, 0.4) );
    
    objReader obj("inputs/cubo.obj");
    objReader obj2("inputs/cubo2.obj");
    auto cubo = new malha(obj);
    auto cubo2 = new malha(obj2);
    cena.add_obj( cubo );
    cena.add_obj( cubo2 );
    
    /*************************************/
    // matrizes de operadores afins
    matrix<4, 4> translacao ({
        {1, 0, 0, 0},
        {0, 1, 0, 5},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    });
    cubo2->affine_transform(translacao);
    cubo2->get_centroid().print();
    
    // objReader objm("inputs/monkey.obj");
    // auto macaco = new malha(objm);
    // cena.add_obj( macaco );
    // macaco->affine_transform(translacao);

    return cena;
}


Scene get_scene_sample(){
    point pos (10, 5, 15);
    point mira(0, 0, 0); 
    vetor up (0, 1, 0);
    double dist = 0.75;
    const int v_res=1080;
    const int h_res=1920;
    point luzpos = point(5, 10, 13);

    Scene cena(Camera(pos, mira, up, dist, v_res, h_res), Luz(Color(0.01, 0.01, 0.01)));

    /*************************************/
    // Adicionar objetos na cena
    cena.luzes.emplace_back(Color(0.7, 0.0, 0.0), point(+20, 3, +20));
    cena.luzes.emplace_back(Color(0.0, 0.7, 0.0), point(+20, 3, -20));
    cena.luzes.emplace_back(Color(0.0, 0.0, 0.7), point(-20, 3, +20));
    cena.luzes.emplace_back(Color(0.3, 0.3, 0.3)*0.25, luzpos);

    cena.add_obj(new plano(point(0, -7, 0), vetor(0, 1, 0), Color(0.3, 0.8, 0.4)));
    cena.objetos.back()->setka( Color(0.3, 0.8, 0.4) );
    // objs.emplace_back(new tabuleiro(point(0, -7, 0), vetor(0, 1, 0), vetor(1, 0, 0), vetor(0.3, 0.8, 0.4), vetor(1, 1, 1)));
    
    objReader obj("inputs/cubo.obj");
    objReader obj2("inputs/cubo2.obj");
    auto cubo = new malha(obj);
    auto cubo2 = new malha(obj2);
    cena.add_obj( cubo );
    cena.add_obj( cubo2 );

    cena.add_obj(new esfera(cubo->get_centroid() + vetor(-3, 2, 3), 0.75, vetor(0.1, 0.2, 0.9)));
    cena.add_obj(new esfera(cubo->get_centroid() + (luzpos - cubo->get_centroid())*-0.5 + vetor(0, 0, 2.95), 1, vetor(0.4, 0.8, 0.6)));
    
    objReader objm("inputs/monkey.obj");
    auto macaco = new malha(objm);
    cena.add_obj( macaco );

    /*************************************/
    // matrizes de operadores afins
    matrix<4, 4> m ({
        {2, 0, 0,  0},
        {0, 1, 0, -3},
        {0, 0, 1,  4},
        {0, 0, 0,  1},
    });
    matrix<4, 4> translacao ({
        {1, 0, 0, 0},
        {0, 1, 0, 5},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    });

    macaco->affine_transform(m);
    cubo2->affine_transform(translacao);

    return cena;
}

#endif

/*
int main(){
    // Adicionar objetos na cena
    //luzes.emplace_back(Color(0.7, 0.0, 0.0), point(+20, 3, +20));
    //luzes.emplace_back(Color(0.0, 0.7, 0.0), point(+20, 3, -20));
    //luzes.emplace_back(Color(0.0, 0.0, 0.7), point(-20, 3, +20));
    luzes.emplace_back(Color(1, 1, 1), luzpos);

    objs.emplace_back(new plano(point(0, -7, 0), vetor(0, 1, 0), Color(0.3, 0.8, 0.4)));
    objs.back()->setka( Color(0.3, 0.8, 0.4) );
    // objs.emplace_back(new tabuleiro(point(0, -7, 0), vetor(0, 1, 0), vetor(1, 0, 0), vetor(0.3, 0.8, 0.4), vetor(1, 1, 1)));
    
    objReader obj("inputs/cubo.obj");
    objReader obj2("inputs/cubo2.obj");
    auto cubo = new malha(obj);
    auto cubo2 = new malha(obj2);
    objs.emplace_back( cubo );
    objs.emplace_back( cubo2 );
    // obj.print_faces_k();

    // objs.emplace_back(new esfera(point(10000, -2000, -8000), 1500, vetor(1, 1, 0)));
    // objs.emplace_back(new esfera(mira, 50, vetor(1, 0, 0)));
    // objs.emplace_back(new esfera(cubo->get_centroid() + vetor(-3, 2, 3), 0.75, vetor(0.1, 0.2, 0.9)));
    // objs.emplace_back(new esfera(cubo->get_centroid() + (luzpos - cubo->get_centroid())*-0.5 + vetor(0, 0, 2.95), 1, vetor(0.4, 0.8, 0.6)));
    
    objReader objm("inputs/monkey.obj");
    auto macaco = new malha(objm);
    // objs.emplace_back( macaco );
    // macaco->affine_transform(m);

    cubo2->affine_transform(translacao);
    // cubo->affine_transform(m);
    // cubo->affine_transform(r);

    objs.emplace_back(new esfera(point(10000, -2000, -8000), 1500, vetor(1, 1, 0)));
    objs.emplace_back(new esfera(mira, 50, vetor(1, 0, 0)));
    objs.emplace_back(new esfera(point(1000, 0, 2000), 500, vetor(1, 0, 0)));
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                objs.emplace_back(new esfera(point(i*300, j*300, k*300), 75 , vetor(0.5*i, 0.5*j, 0.5*k)));
*/