#ifndef VEC2_H
#define VEC2_H
#include <iostream>
#include "Vec3.h"

class Vec2
{
    public:
        Vec2();
        Vec2(double _x, double _y);
        Vec2(double _n);
        Vec2(const Vec2& _Vec2);
        Vec2(const Vec3& _Vec3);
        void cast_uint();
        inline const double tupleMax(){return std::max(x,y);}
        ~Vec2();
        friend std::ostream& operator<<(std::ostream& os, Vec2 _Vec2);
        Vec2& operator=(const Vec2& _Vec2);
        Vec2& operator=(const double& num);
        const Vec2 operator/(const double& rv) const;
        const Vec2 operator/(const int& rv) const;
        Vec2& operator+=(const Vec2& _Vec2);
        bool operator==(const Vec2& _Vec2);
        double& operator()(const int _i) {return (_i == 0) ? x : y;}
        const double& operator()(const int _i) const {return (_i == 0) ? x : y;}
        const double& operator[](const int _i) const {return (_i == 0) ? x : y;}
        const Vec2 operator-(const Vec2& _Vec2) const;
        const Vec2 operator+(const Vec2& _Vec2) const;
        inline double Max() const {return std::max(this->x,this->y);}
        inline double Min() const {return std::min(this->x,this->y);}
        Vec2 Maxof2(Vec2& _Vec2) const;
        Vec2 Minof2(Vec2& _Vec2) const;
        // Coordinates
        double x,y,z;
};

#endif