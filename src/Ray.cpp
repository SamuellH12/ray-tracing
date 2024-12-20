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

    //Implemente os métodos aqui

    //Getters
    const point get_origin() const { return origin; }
    const vetor get_direction() const { return direction; }
};

#endif