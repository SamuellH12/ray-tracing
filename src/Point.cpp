#ifndef POINTHEADER
#define POINTHEADER
#include <iostream>
#include "Vector.cpp"

/*
Classe de pontos.
A classe precisa ser instanciada passando as componentes x, y e z
*/
/*
Operações com pontos
ok diferença -> vetor
ok soma com vetor -> pt
ok sub vetor pt -> pt
*/

class point{
    private:
        double x;
        double y;
        double z;
public:

    //Construtores
    point() {}
    point(double x, double y, double z) : x(x), y(y), z(z) {}

    //Implemente os métodos de pontos aqui
    vetor  operator- (const point&a) const{ return vetor(x-a.x, y-a.y, z-a.z); }  
    point  operator+ (const vetor&v) const{ return point(x+v.getX(), y+v.getY(), z+v.getZ()); }  //soma c/ vetor
    point  operator- (const vetor&v) const{ return point(x-v.getX(), y-v.getY(), z-v.getZ()); }  //sub  c/ vetor

    //Print do vetor no formato (x, y, z)
    void print(){ std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl; }

    //Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }   
};

#endif