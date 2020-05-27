#ifndef VEC3_H
#define VEC3_H
#include <iostream>

class Vec3
{
    public:
        Vec3();
        Vec3(char* facet);
        Vec3(double _x, double _y, double _z);
        Vec3(double _n);
        Vec3(const Vec3& _Vec3);
        void cast_uint();
        Vec3& cast2int();
        inline const double tupleMax(){return std::max(std::max(x,y),z);}
        ~Vec3();
        friend std::ostream& operator<<(std::ostream& os, Vec3 _Vec3);
        Vec3& operator=(const Vec3& _Vec3);
        const Vec3 operator/(const double& rv) const;
        Vec3& operator+=(const Vec3& _Vec3);
        Vec3& round2int();
        Vec3& ceiling();
        double& operator()(const int _i) {return (_i == 0) ? x : ((_i == 1) ? y : z);}
        const double& operator()(const int _i) const {return (_i == 0) ? x : ((_i == 1) ? y : z);}
        const double& operator[](const int _i) const {return (_i == 0) ? x : ((_i == 1) ? y : z);}
        const Vec3 operator-(const Vec3& _Vec3) const;
        // Coordinates
        double x,y,z;
};

#endif