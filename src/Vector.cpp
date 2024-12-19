#ifndef VECTORHEADER
#define VECTORHEADER
#include <iostream>
#include <math.h>
#include "Point.cpp"

/*
Classe de vetores.
A classe precisa ser instanciada passando as componentes x, y e z
*/
/*
Operações a implementar:
Soma de vetores -> vetor
subtração de vetores -> vetor
mult por escalar -> vetor
div por escalar -> vetor
produto interno -> escalar
Get perpendicular to vector
get perpendicular to plane
*/


class vetor{
    private:
        double x;
        double y;
        double z;
    
    public:

    //Construtores
    vetor() {}
    vetor(double x, double y, double z): x(x), y(y), z(z) {}

    //Implemente os métodos de vetores aqui
    vetor  operator+ (const vetor&v) const{ return vetor(x+v.x, y+v.y, z+v.z); }
    vetor  operator- (const vetor&v) const{ return vetor(x-v.x, y-v.y, z-v.z); }  
    double operator* (const vetor&v) const{ return  x*v.x + y*v.y + z*v.z; }  //DOT product / Produto interno
    vetor  operator* (double c) const{ return vetor(x*c, y*c, z*c); } //mult por escalar
    vetor  operator/ (double c) const{ return vetor(x/c, y/c, z/c); } //div por escalar

    //Print do vetor no formato <x, y, z>
    void print(){ std::cout << "<" << x << ", " << y << ", " << z << ">" << std::endl; }

    //Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
};

#endif