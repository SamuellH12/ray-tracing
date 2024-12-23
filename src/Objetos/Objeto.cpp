#include <iostream>
#include <math.h>
#include "../Point.cpp"
#include "../Vector.cpp"
#include "../Ray.cpp"
#include <vector>
#include <tuple>
#pragma once

class objeto {
private:

protected:
point pos;
vetor color;

public:
objeto(){}
objeto(point pos, vetor color) : pos(pos), color(color) {}

virtual bool has_intersection(ray r, double &t) = 0;

vetor get_color() {return color;}

};