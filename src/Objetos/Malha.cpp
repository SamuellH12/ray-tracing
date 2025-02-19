#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include "../ObjReader.cpp"
#include "../Matrix.cpp"
#include "Objeto.cpp"
#include <vector>
#include <tuple>

class malha : public objeto {
private:
    // // // point pos; vetor color;
    std::vector<point> vertices;
    std::vector<vetor> normals;
    std::vector<Face> faces;

    Intersection get_face_intersection(int id_face, ray &r){
        Face &f = faces[id_face];
        point &a = vertices[f.verticeIndice[0]];
        point &b = vertices[f.verticeIndice[1]];
        point &c = vertices[f.verticeIndice[2]];
        vetor normal = (b-a) % (c-a);
        normal = normal.normalized();

        double n = normal * r.get_direction();

        if(n == 0.0) return Intersection();

        double d = normal * (a - r.get_origin());
        double t = (d/n);

        if(t <= 0) return Intersection();
        //tem intersecao com o plano

        point i = r.get_origin() + r.get_direction() * t;

        if( ((b-a) % (i - a)) * normal < 0 ) return Intersection();
        if( ((c-b) % (i - b)) * normal < 0 ) return Intersection();
        if( ((a-c) % (i - c)) * normal < 0 ) return Intersection();

        //a intersecao esta dentro do triangulo
        return Intersection(t, normal, color);
    }

public:
    malha(){}
    malha(objReader &obj) : objeto(point(), obj.getKd()) {
        vertices = obj.getVertices();
        normals = obj.getNormals();
        faces = obj.getFaces();
    }


    Intersection get_intersection(ray &r) override{ 
        Intersection ans;

        for(int i=0; i<faces.size(); i++)
            ans = min<Intersection>(ans, get_face_intersection(i, r));

        return ans;
    }

    void affine_transform(matrix<4, 4> m){
        for(auto &p : vertices) p = m * p;
        for(auto &v : normals)  v = m * v;
    }
};