#include "Vec3.h"
#include <cmath>

Vec3::Vec3(){}

Vec3::Vec3(char* facet){
    char f1[4] = {facet[0], facet[1], facet[2], facet[3]};
    char f2[4] = {facet[4], facet[5], facet[6], facet[7]};
    char f3[4] = {facet[8], facet[9], facet[10], facet[11]};
    float xx = *((float*)f1);
    float yy = *((float*)f2);
    float zz = *((float*)f3);
    x = double(xx);
    y = double(yy);
    z = double(zz);
}

Vec3::Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z){
}

Vec3::Vec3(const Vec3& _Vec3){
    this->x = _Vec3.x;
    this->y = _Vec3.y;
    this->z = _Vec3.z;
}

Vec3::Vec3(double _n) : x(_n), y(_n), z(_n){
}

void Vec3::cast_uint() {
    this->x = static_cast<uint>(this->x);
    this->y = static_cast<uint>(this->y);
    this->z = static_cast<uint>(this->z);
}

Vec3& Vec3::cast2int() {
    this->x = static_cast<int>(this->x);
    this->y = static_cast<int>(this->y);
    this->z = static_cast<int>(this->z);
    return *this;
}

std::ostream& operator<<(std::ostream& os, Vec3 _Vec3) {
	os << "( " << _Vec3.x << ", " << _Vec3.y << ", " << _Vec3.z << " )";
	return os;
}

Vec3& Vec3::operator=(const Vec3& _Vec3){
    this->x = _Vec3.x;
    this->y = _Vec3.y;
    this->z = _Vec3.z;
    return *this;
}

Vec3& Vec3::operator+=(const Vec3& _Vec3){
    this->x += _Vec3.x;
    this->y += _Vec3.y;
    this->z += _Vec3.z;
    return *this; 
}

Vec3& Vec3::round2int(){
    this->x = static_cast<int>(std::round(this->x));
    this->y = static_cast<int>(std::round(this->y));
    this->z = static_cast<int>(std::round(this->z));
    return *this;
}

Vec3& Vec3::ceiling(){
    this->x = static_cast<int>(std::ceil(this->x));
    this->y = static_cast<int>(std::ceil(this->y));
    this->z = static_cast<int>(std::ceil(this->z));
    return *this;
}

const Vec3 Vec3::operator/(const double& _v) const {
    return Vec3((x / _v), (y / _v), (z / _v));
}

const Vec3 Vec3::operator-(const Vec3& _Vec3) const {
	return Vec3(x - _Vec3.x, y - _Vec3.y, z - _Vec3.z);
}

Vec3::~Vec3(){}

