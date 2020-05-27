#include "Vec2.h"
#include <cmath>

Vec2::Vec2(){}

Vec2::Vec2(double _x, double _y) : x(_x), y(_y){
}

Vec2::Vec2(const Vec2& _Vec2){
    this->x = _Vec2.x;
    this->y = _Vec2.y;
}

Vec2::Vec2(const Vec3& _Vec3){
    this->x = _Vec3.x;
    this->y = _Vec3.y;
}

Vec2::Vec2(double _n) : x(_n), y(_n){
}

void Vec2::cast_uint() {
    this->x = static_cast<uint>(this->x);
    this->y = static_cast<uint>(this->y);
}

std::ostream& operator<<(std::ostream& os, Vec2 _Vec2) {
	os << "( " << _Vec2.x << ", " << _Vec2.y << " )";
	return os;
}

Vec2& Vec2::operator=(const Vec2& _Vec2){
    this->x = _Vec2.x;
    this->y = _Vec2.y;
    return *this;
}

Vec2& Vec2::operator=(const double& num){
    this->x = num;
    this->y = num;
    return *this;
}

Vec2& Vec2::operator+=(const Vec2& _Vec2){
    this->x += _Vec2.x;
    this->y += _Vec2.y;
    return *this;
}

bool Vec2::operator==(const Vec2& _Vec2){
    if (this->x == _Vec2.x && this->y == _Vec2.y) {
        return true; 
    } else {
        return false;
    }

}

const Vec2 Vec2::operator/(const double& _v) const {
    return Vec2(x / _v, y / _v);
}

const Vec2 Vec2::operator/(const int& _v) const {
    return Vec2(int(x / _v), int(y / _v));
}

const Vec2 Vec2::operator-(const Vec2& _Vec2) const {
	return Vec2(x - _Vec2.x, y - _Vec2.y);
}

const Vec2 Vec2::operator+(const Vec2& _Vec2) const {
	return Vec2(x + _Vec2.x, y + _Vec2.y);
}

Vec2 Vec2::Maxof2(Vec2& _Vec2) const {
    _Vec2.x = std::max(_Vec2.x, this->x);
    _Vec2.y = std::max(_Vec2.y, this->y);
    return _Vec2;
}

Vec2 Vec2::Minof2(Vec2& _Vec2) const {
    _Vec2.x = std::min(_Vec2.x, this->x);
    _Vec2.y = std::min(_Vec2.y, this->y);
    return _Vec2;
}

Vec2::~Vec2(){}

