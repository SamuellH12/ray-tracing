#ifndef RAYHEADER
#define RAYHEADER
#include <iostream>
#include <math.h>
#include "Point.cpp"
#include "Vector.cpp"

/*
ray
*/

class ray{
    private:
        point origin;
        vetor direction;
    
    public:

    //Construtores
    ray() {}
    ray(point origin, vetor direction): origin(origin), direction(direction) {
        direction = direction.normalized();
    }
    ray(point origin, point mira): origin(origin), direction(mira - origin) {
        direction = direction.normalized();
    }

    //Implemente os métodos aqui

    //Getters
    const point get_origin() const { return origin; }
    const vetor get_direction() const { return direction; }
    const point get_point(double t){ return origin + direction*t; }
};

#endif