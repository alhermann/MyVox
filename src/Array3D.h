#ifndef ARRAY3D_H
#define ARRAY3D_H

#include <iostream>
#include "Vec3.h"

template <typename T_Type>
class Array3D
{
    public:
        T_Type *m_Data = NULL;
        int m_xSize;
        int m_ySize;
        int m_zSize;
        int m_Size;

        Array3D(void) : m_xSize(0), m_ySize(0), m_zSize(0), m_Size(0){}

        Array3D(int xSize, int ySize, int zSize) : m_xSize(xSize), m_ySize(ySize), m_zSize(zSize), m_Size(xSize*ySize*zSize){
            this->allocate(m_Size);
        }
        Array3D(const Vec3& sizes) : m_xSize(sizes.x), m_ySize(sizes.y), m_zSize(sizes.z), m_Size(sizes.x*sizes.y*sizes.z){
            this->allocate(m_Size);
        }
        ~Array3D(void){}
        void erase(){
            if (m_Data != NULL){
                delete[] (m_Data);
                m_Data = NULL;
            }
        }
        // 3D Array xsize
        unsigned int xsize() const {
            return this->m_xSize;
        }
        // 3D Array ysize
        unsigned int ysize() const {
            return this->m_ySize;
        }
        // 3D Array zsize
        unsigned int zsize() const {
            return this->m_zSize;
        }
        void allocate(int size_to_allocate) {
            erase();
            this->m_Data = new T_Type[size_to_allocate];
        }
        void fill(T_Type value_to_fill_with){
            for (int n = 0; n < this->m_Size; n++){
                m_Data[n] = value_to_fill_with;
            }
        }
        inline const int get_size(){return this->m_Size;}
        friend std::ostream& operator<<(std::ostream& os, Array3D<T_Type> _m){
            os << "( ";
            for (int i = 0; i < _m.get_size(); i++){
                os << static_cast<unsigned>(_m.m_Data[i]) << " ";
            }
            os << ")" << "\n";
            return os;
        }
        T_Type& at(unsigned int x, unsigned int y, unsigned int z) const {
            return (m_Data[(z*m_ySize + y) * m_xSize + x]);
        }
};

#endif