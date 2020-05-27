#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Vec3.h"

class Triangle
{
    public:
        Triangle();
        Triangle(Vec3 _norm, Vec3 _p1, Vec3 _p2, Vec3 _p3, double _idx1, double _idx2, double _idx3);
        ~Triangle();
        Vec3 norm, p1, p2, p3;
        double idx1, idx2, idx3;
};

#endif