#ifndef VECTORHEADER
#define VECTORHEADER
#include <iostream>
#include <math.h>
// #include "Point.cpp"

/*
Classe de vetores.
A classe precisa ser instanciada passando as componentes x, y e z
*/
/*
Operações a implementar:
ok Soma de vetores -> vetor
ok subtração de vetores -> vetor
ok mult por escalar -> vetor
ok div por escalar -> vetor
ok produto interno -> escalar
Get perpendicular to vector
get perpendicular to plane
*/
class point;

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
    vetor  operator% (vetor v) const {  return vetor(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); } // cross product 
    vetor normalized(){ return *this / sqrt((*this)*(*this)); }

    //Print do vetor no formato <x, y, z>
    void print(){ std::cout << "<" << x << ", " << y << ", " << z << ">" << std::endl; }

    //Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
};

#endif