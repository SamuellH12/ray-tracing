#ifndef SCENEHEADER
#define SCENEHEADER
#include "../src/ObjReader.cpp"
#include "../src/Vector.cpp"
#include "../src/Luz.cpp"
#include "../src/Objetos/Esfera.cpp"
#include "../src/Objetos/Plano.cpp"
#include "../src/Objetos/Tabuleiro.cpp"
#include "../src/Objetos/Malha.cpp"
#include "../src/Objetos/BezierLine.cpp"
#include "../src/Objetos/Cilindro.cpp"
#include "sceneObj.cpp"
#include <vector>

Scene get_scene_debug(point camPos = point(20, 10, 10), point luzPos = point(10, 3, 10)){
    point mira(10, 0, 10); 
    vetor up (1.1, 0, 0);
    double dist = 0.75;
    const int v_res=1080/2;
    const int h_res= 1920/2;

    Scene cena(Camera(camPos, mira, up, dist, v_res, h_res), Luz(Color(0.01, 0.01, 0.01)));

    /*************************************/
    // Adicionar objetos na cena
    cena.luzes.emplace_back(Color(1, 1, 1), luzPos);

    // cena.add_obj(new plano(point(-1000, 10000, -1000), vetor(0, -1, 0), Color(0.3, 0.8, 0.4)));
    // cena.objetos.back()->setke(Color(1, 1, 1));


    cena.add_obj(new plano(point(0, -7, 0), vetor(0, 1, 0), Color(0.3, 0.8, 0.4)));
    cena.objetos.back()->setka( Color(0.3, 0.8, 0.4) );
    cena.objetos.back()->setke(Color(0.7, 0.7, 0.7))  ;
    
    cena.add_obj(new esfera(mira, 0.75, vetor(0.1, 0.2, 0.9)));

    objReader obj("inputs/cubo.obj");
    objReader obj2("inputs/cubo2.obj");
    auto cubo = new malha(obj);
    auto cubo2 = new malha(obj2);
    // cena.add_obj( cubo );
    // cena.add_obj( cubo2 );
    
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


Scene get_scene_classic(int v_res=600, int h_res= 800, point camPos = point(-322, 34, -182), point luzPos = point(11, 65, 10)){
    point mira(0, 0, 0); 
    vetor up (0, 1, 0);
    double dist = 0.75;

    Scene cena(Camera(camPos, mira, up, dist, v_res, h_res), Luz(Color(0.25, 0.25, 0.25)));

    cena.luzes.emplace_back(Color(0.75, 0.75, 0.75), luzPos);
    // cena.luzes.emplace_back(Color(1, 0, 0), point(-50, 15,  20));
    // cena.luzes.emplace_back(Color(0, 1, 0), point(+50, 15,  20));
    // cena.luzes.emplace_back(Color(0, 0, 1), point(-50, 15, -20));
    
    /*************************************/
    // Adicionar objetos na cena
    // cena.add_obj(new Bline(std::vector<point>({point(0, 0, 0), point(10, 42, 45), point(25, 12, 35), point(-54, -10, 20)}), Color(0.2, 0.9, 0.3), 5));
    // cena.add_obj(new Bline(std::vector<point>({point(0, 0, 0), point(0, 10, 0)}), Color(0.2, 0.9, 0.3), 1));
    
    
    cena.add_obj(new plano(point(1000, 70, 10000), vetor(0, 1, 0), Color(0, 0.2, 0.9)));
    cena.objetos.back()->setke( Color(1, 1, 1) );


    // cena.add_obj(new esfera(point(1000, -200, -800), 150, vetor(1, 1, 0)));
    
    // // cena.add_obj(new tabuleiro(point(0, -10, 0), vetor(0, 1, 0), vetor(1, 0, 0), vetor(0.3, 0.8, 0.4), vetor(1, 1, 1)));
    cena.add_obj(new plano(point(0, -10, 0), vetor(0, 1, 0), Color(0.3, 0.8, 0.4)));
    cena.objetos.back()->setke( Color(0.75, 0.75, 0.75) );
    // // cena.objetos.back()->setka( Color(0.3, 0.8, 0.4) );
    // cena.objetos.back()->setka(Color());
    // cena.objetos.back()->setks(Color());
    // cena.objetos.back()->setkd(Color());
    cena.objetos.back()->setd(0.1);
    cena.objetos.back()->setni(1.1);

    vetor dlt(-300, 0, 0);
    cena.add_obj(new cilindro(point(200, -50, 53) + dlt, point(239, 20, 71)+dlt, 5, Color(0.7, 0.7, 0.1), true));

    // cena.add_obj(new esfera(point(150, 5, 120), 8, Color(0.8, 0.7, 0.2)));

    for(int i=0; i<3; i+=2)
        for(int j=0; j<3; j+=2)
            for(int k=0; k<3; k+=2){
                cena.add_obj(new esfera(point(i*30 - 30, j*30, k*30 - 30), 7.5, Color(0.5*i, 0.5*j, 0.5*k)));
                Color cl(0.25, 0.25, 0.75);
                // cena.objetos.back()->setkd(cl * i/3.0);
                cena.objetos.back()->setka(cl * j/3.0);
                // cena.objetos.back()->setks(cl * k/3.0);
            }

    objReader objm("inputs/cubo2.obj");
    auto macaco = new malha(objm);
    cena.add_obj( macaco );
    matrix<4, 4> m ({
        {20, 0, 0,  0},
        {0, 20, 0,  20*sqrt(2)},
        {0, 0, 20,  0},
        {0, 0, 0,  1},
    });
    macaco->affine_transform(m * MatrixXRotation(PI/4) * MatrixZRotation(PI/4) );
    cena.add_obj(new esfera(macaco->get_centroid(), 0.5, Color(0, 0, 1)));


    Color RED(1, 0, 0);
    cena.add_obj(new esfera(point(70, 0, 170), 50, RED));
    cena.objetos.back()->setkd( RED );
    cena.objetos.back()->setka( RED );
    cena.objetos.back()->setks( RED );

    cena.add_obj(new esfera(point(-100, 5, -140), 10, Color()));
    cena.objetos.back()->setke( Color(0.75, 0.75, 0.75) );


    cena.add_obj(new esfera(point(-200, 5, -170), 10, Color()));
    cena.objetos.back()->setka(Color());
    cena.objetos.back()->setks(Color());
    cena.objetos.back()->setkd(Color());
    cena.objetos.back()->setd(0.01);
    cena.objetos.back()->setni(1.01);

    // point cpos(1500, 0, 300);
    // double csz = 500;
    // double cr = 50;
    // Color ccl (0.8, 0.3, 0.6);
    // vetor nxt(csz, 0, 0);
    // bool preenchido = false;

    // for(int j=0; j<2; j++){
    //     for(int i=0; i<4; i++){
    //         objs.emplace_back(new cilindro(cpos, cpos+nxt, cr, ccl, preenchido));
    //         cpos = cpos+nxt;
    //         nxt = nxt.rotatey(PI/2.0);
    //         if(!j) objs.emplace_back(new cilindro(cpos, cpos + vetor(0, csz, 0) , cr, ccl, preenchido));
    //     }

    //     cpos = cpos + vetor(0, csz, 0);
    // }


    return cena;
}


#endif