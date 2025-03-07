#ifndef LUZHEADER
#define LUZHEADER
#include "Vector.cpp"
#include "Point.cpp"

class Luz {
    public:
    point pos;
    Color I;
    Luz(){};
    Luz(Color I) : I(I) {};
    Luz(Color I, point pos) : I(I), pos(pos) {};
};

#endif