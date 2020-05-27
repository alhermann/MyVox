#include "Triangle.h"
 
Triangle::Triangle() {}

Triangle::Triangle(Vec3 _norm, Vec3 _p1, Vec3 _p2, Vec3 _p3, double _idx1, double _idx2, double _idx3) : norm(_norm), p1(_p1), p2(_p2), p3(_p3), idx1(_idx1), idx2(_idx2), idx3(_idx3) {}

Triangle::~Triangle() {}