#ifndef MAT4_H
#define MAT4_H

#include "Vec3.h"
#include <iostream>

template <typename T = float, typename I = unsigned int>
class Matrix4d{
    public:
    Matrix4d() : _rows(4), _cols(4){
        _data = new T[this->_rows * this->_cols];
        this->setZeros();
    }
    ~Matrix4d(){
        if (_data != 0){
            delete[] _data;
        }
    };
    Matrix4d(const Matrix4d &_m){
        this->_rows = _m._rows;
        this->_cols = _m._cols;
        this->_data = new T[this->_rows * this->_cols];
        for (int i = 0; i < this->_rows; i++){
            for (int j = 0; j < this->_cols; j++){
                this->_data[i * this->_cols + j] = _m._data[i * this->_cols + j];
            }
        }
    }
    Matrix4d& operator=(const Matrix4d &_m){
        this->clear();
        for (int i = 0; i < _rows * _cols; i++){
            _data[i] = _m._data[i];
        }  
        return *this; 
    }
    Matrix4d operator*(const Matrix4d &_m){
        double sum_elem;
        for(int i = 0; i < this->_rows; i++){
            for(int j = 0; j < _m.cols(); j++){
                sum_elem = 0.0;
                for(int k = 0; k < this->_rows; k++){
                    sum_elem += this->_data[i * this->_cols + k] * _m(k,j);
                }
                this->_data[i * this->_cols + j] = sum_elem;
            }
        }
        return *this;  
    }
    friend std::ostream& operator<<(std::ostream& os, Matrix4d _m){
        for (int i = 0; i < _m.rows(); i++){
            os << "( ";
            for (int j = 0; j < _m.cols(); j++){
                os << _m._data[i * _m.cols() + j] << " ";
            }
            os << ")" << "\n";
        }
        return os;
    }
    const T& at(I _c, I _r) const {
        return this->_data[_c+_r*4];
    }
    T& at(I _c, I _r) {
        return this->_data[_c+_r*4];
    }

    Vec3 mulPoint(const Vec3& p) const{
        Vec3 r(0.0);
        for (int i = 0; i < 3; i++){
            for (int k = 0; k < 3; k++){
                r(i) += at(k,i)*p(k);
            }
            r(i) += at(3,i);
        }
        return r;
    }

    Vec3 mulPointFloat(const Vec3& p) const{
        Vec3 r(0.f);
        for (int i = 0; i < 3; i++){
            for (int k = 0; k < 3; k++){
                r(i) += at(k,i)*p(k);
            }
            r(i) += at(3,i);
            r(i) = (float)r(i);
        }
        return r;
    }

    inline void clear(){
        for (int i = 0; i < _rows * _cols; i++){
            _data[i] = T();
        }
    }
    inline T &operator()(const I _i, const I _j) {
        return _data[_i * _cols + _j];
    }
    inline const T &operator()(const I _i, const I _j) const {
        return _data[_i * _cols + _j];
    }
    inline void setZeros(){
        this->clear();
        for (int i = 0; i < _rows * _cols; i++){
            _data[i] = 0.f;
        }       
    }
    inline void setOnes(){
        this->clear();
        for (int i = 0; i < _rows; i++){
            for (int j = 0; j < _cols; j++){
                if (i==j){
                    _data[i * _cols + j] = 1.f;
                }
                else {
                    _data[i * _cols + j] = 0.f;
                }
            }
        }
    }
    void scaleMatrix(T _scale){
        this->setOnes();
        for (int i = 0; i < _rows - 1; i++){
            for (int j = 0; j < _cols - 1; j++){
                if (i==j){
                    _data[i * _cols + j] = _scale;
                }
            }
        }
    }
    void scaleMatrix(Vec3 _scale){
        this->setOnes();
        _data[0 * _cols + 0] = _scale.x;
        _data[1 * _cols + 1] = _scale.y;
        _data[2 * _cols + 2] = _scale.z;
    }
    inline void translationMatrix(Vec3 _Vec3){
        this->setOnes();
        _data[0 * _cols + 3] = _Vec3.x;
        _data[1 * _cols + 3] = _Vec3.y;
        _data[2 * _cols + 3] = _Vec3.z;
    }
    inline I rows() const {return _rows;};
    inline I cols() const {return _cols;};
    inline T* data()const {return _data;};
    private:
        T* _data;
        I _rows;
        I _cols;
};

Matrix4d<> scaleMatrix(Vec3 _scale){
    Matrix4d<> _m;
    _m.setOnes();
    _m(0,0) = _scale.x;
    _m(1,1) = _scale.y;
    _m(2,2) = _scale.z;
    return _m;
}

Matrix4d<> translationMatrix(Vec3 _Vec3){
    Matrix4d<> _m;
    _m.setOnes();
    _m(0,3) = _Vec3.x;
    _m(1,3) = _Vec3.y;
    _m(2,3) = _Vec3.z;
    return _m;
}

#endif