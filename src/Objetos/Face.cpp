#ifndef FACEHEADER
#define FACEHEADER
#include <vector>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "Objeto.cpp"

struct Face {
    int verticeIndice[3];
    int normalIndice[3];
    vetor ka;
    vetor kd;
    vetor ks;
    vetor ke;
    vetor normal;
    double ns;
    double ni;
    double d;

    Color kt; // transmissão // double ni; //Optical density // double d; // Kt=(1−d) ⋅ 4/(Ni+1)^2
    // Color kd; // difuso
    // Color ks; // specular
    // Color ka; // ambiental
    // Color kr; // reflexao
    // Color ke; // Emissive
    // double ns;

    Face() {
        for (int i = 0; i < 3; ++i) {
            verticeIndice[i] = 0;
            normalIndice[i] = 0;
        }
        ka = vetor();
        kd = vetor();
        ks = vetor();
        ke = vetor();
        ns = 0.0;
        ni = 0.0;
        d = 0.0;
    }

    void recalc_normal(std::vector<point> &vertices){
        normal = (vertices[verticeIndice[1]] - vertices[verticeIndice[0]]) % 
                 (vertices[verticeIndice[2]] - vertices[verticeIndice[0]]);
        normal = normal.normalized();
    }

    Color get_color(ray &r, point p, vetor normal, Luz const &Ia, std::vector<Luz> const &luzes, std::vector<objeto*> const &objetos, int profundidade){ 
        if(profundidade <= 0) return Color(0, 0, 0);
        
        Color I = ka^Ia.I;
        point pt = p + (r.get_direction() * -1 * (1e-6));
        normal = normal.normalized();
        for(auto [Pi, Ii] : luzes)
        {
            vetor Li = Pi - pt;
            double tmax = Li.norm();
            Li = Li.normalized();
            ray q (pt, Li);
            vetor Ri = (normal*2.0*(normal*Li) - Li).normalized();

            double f = 1;
            for(auto obj : objetos)
                if(obj->has_intersection(q, tmax)){
                    f = f * ( 1.0 - obj->getd() );
                    if(f < 1e-8) break;
                }

            if(f < 1e-8) continue;
            vetor v = r.get_direction()*-1;

            auto newI = ((Ii ^ kd) * std::max(normal*Li, 0.0)) + ((Ii ^ ks) * pow(std::max(Ri*v, 0.0), ns));
            I = I + newI*f;
        }

        Intersection IR;
        Intersection IT;

        
        // reflexão
        if(ke.norm2() > 0) //só calcula se o objeto for reflexivo
        {
            vetor rv = r.get_direction() * -1;
            vetor Rf = (normal*2.0*(normal*rv) - rv).normalized();
            double distRef = DOUBLEINF;
            objeto* obr = NULL;
            
            ray rayR (pt, Rf);
            for(auto obj : objetos)
            {
                double dt = obj->dist_intersection(rayR);
                if(dt < distRef && dt > 0.0)
                    distRef = dt,
                    obr = obj;
                // IR = std::min(IR, obj->get_intersection(rayR, Ia, luzes, objetos, profundidade-1) );
            }
            
            if(obr) IR = obr->get_intersection(rayR, Ia, luzes, objetos, profundidade-1);
        }

        // refração
        if(d < 1.0) // só calcula se o objeto for transparente
        {
            vetor rv = r.get_direction();
            double n1 = 1;
            double n2 = ni;
            double n = n2/n1;
            double seni = (rv%normal).norm();
            double senr = seni/n2; //sin O_t
            double cosot = sqrt(1 - senr*senr);
            double coso  = sqrt(1 - seni*seni);
            vetor T = rv/n  - normal*(cosot - coso/n);
            ray rayT (p + (r.get_direction() * (1e-6)), T);
            
            double distTr = DOUBLEINF;
            objeto* obt = NULL;

            for(auto obj : objetos){
                double dt = obj->dist_intersection(rayT);
                if(dt < distTr && dt > 0.0)
                    distTr = dt,
                    obt = obj;
                // IT = std::min(IT, obj->get_intersection(rayT, Ia, luzes, objetos, profundidade-1) );
            }
            
            if(obt) IT = obt->get_intersection(rayT, Ia, luzes, objetos, profundidade-1);
        }

        Color Ir = IR.color;
        Color It = IT.color;

        I = I + (kt ^ It);
        I = I + (ke ^ Ir);

        return I;
    }

    void print_k(){
        ka.print();
        kd.print();
        ks.print();
        std::cerr << "----\n";
    }
};


#endif