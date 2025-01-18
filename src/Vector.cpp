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
    double norm2() const { return (*this)*(*this); }
    vetor normalized() const { return *this / norm(); }

    vetor rotatex(double theta) const {
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        return vetor(x,
                     y * cosTheta - z * sinTheta,
                     y * sinTheta + z * cosTheta );
    }

    vetor rotatey(double theta) const {
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        return vetor( x * cosTheta + z * sinTheta,
                      y,
                     -x * sinTheta + z * cosTheta );
    }

    vetor rotatez(double theta) const {
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        return vetor( x * cosTheta - y * sinTheta,
                      x * sinTheta + y * cosTheta,
                      z );
    }

    vetor rotateAroundAxis(vetor axis, double theta) const {
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        axis = axis.normalized();
        double ax = axis.getX();
        double ay = axis.getY();
        double az = axis.getZ();

        return vetor(
            x * (cosTheta + ax * ax * (1 - cosTheta)) +
            y * (ax * ay * (1 - cosTheta) - az * sinTheta) +
            z * (ax * az * (1 - cosTheta) + ay * sinTheta),

            x * (ay * ax * (1 - cosTheta) + az * sinTheta) +
            y * (cosTheta + ay * ay * (1 - cosTheta)) +
            z * (ay * az * (1 - cosTheta) - ax * sinTheta),

            x * (az * ax * (1 - cosTheta) - ay * sinTheta) +
            y * (az * ay * (1 - cosTheta) + ax * sinTheta) +
            z * (cosTheta + az * az * (1 - cosTheta))
        );
    }


    //Print do vetor no formato <x, y, z>
    void print(){ std::cout << "<" << x << ", " << y << ", " << z << ">" << std::endl; }

    //Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
};

typedef vetor Color;

#endif