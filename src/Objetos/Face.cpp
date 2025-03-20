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
        Color I = ka^Ia.I;
        point pt = p + (r.get_direction() * -1e-9);
        for(auto [Pi, Ii] : luzes)
        {
            vetor Li = Pi - pt;
            double tmax = Li.norm();
            Li = Li.normalized();
            ray q (pt , Li);
            vetor Ri = normal*2.0*(normal*Li) - Li;

            bool ok = true;
            for(auto obj : objetos)
                if(obj->has_intersection(q, tmax)){
                    ok = false;
                    break;
                }

            if(!ok) continue;
            vetor v = r.get_direction()*-1;

            I = I + ((Ii ^ kd) * (normal*Li)) + ((Ii ^ ks) * pow(Ri*v, ns));
        }

        vetor rv = r.get_direction();

        double n1 = 1;
        double n2 = ni;
        double n = n2/n1;
        double seni = (rv%normal).norm();
        double senr = seni/n2; //sin O_t
        double cosot = sqrt(1 - senr*senr);
        double coso  = sqrt(1 - seni*seni);
        vetor T = rv/n  - normal*(cosot - coso/n);
        
        vetor Rf = (normal*2.0*(normal*rv) - rv).normalized();
        
        Intersection IR;
        Intersection IT;
        ray rayR (pt, Rf);
        ray rayT (pt, T);

        for(auto obj : objetos)
        {
            IR = std::min(IR, obj->get_intersection(rayR, Ia, luzes, objetos, profundidade-1) );
            IT = std::min(IT, obj->get_intersection(rayT, Ia, luzes, objetos, profundidade-1) );
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