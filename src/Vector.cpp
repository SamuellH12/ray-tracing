#ifndef VECTORHEADER
#define VECTORHEADER
#include <iostream>
#include <math.h>

class point;

class vetor{
    private:
        double x = 0;
        double y = 0;
        double z = 0;
    
    public:

    //Construtores
    vetor() {}
    vetor(double x, double y, double z): x(x), y(y), z(z) {}

    //operadores
    vetor  operator+ (const vetor&v) const{ return vetor(x+v.x, y+v.y, z+v.z); }
    vetor  operator- (const vetor&v) const{ return vetor(x-v.x, y-v.y, z-v.z); }  
    double operator* (const vetor&v) const{ return  x*v.x + y*v.y + z*v.z; }  //DOT product / Produto interno
    vetor  operator* (double c) const{ return vetor(x*c, y*c, z*c); } //mult por escalar
    vetor  operator/ (double c) const{ return vetor(x/c, y/c, z/c); } //div por escalar
    vetor  operator% (vetor v) const {  return vetor(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); } // cross product 
    
    double norm() const { return sqrt((*this)*(*this)); }
    vetor normalized() const { return *this / norm(); }
    //vetor rotatex(double theta)
    //vetor rotatey(double theta)
    //vetor rotatez(double theta)

    //Print do vetor no formato <x, y, z>
    void print(){ std::cout << "<" << x << ", " << y << ", " << z << ">" << std::endl; }

    //Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }


    // for color only // multiplicação par a par
    vetor operator^ (const vetor&v) const{ return  vetor(x*v.x, y*v.y, z*v.z); }
};

typedef vetor Color;
// class Color : vetor {
    // (const vetor&v) const{ return  x*v.x + y*v.y + z*v.z; }  //DOT product / Produto interno
// };

#endif