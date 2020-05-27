#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <vector>
#include "Triangle.h"

class Geometry {
    public:
        Geometry(std::string fname_);
        ~Geometry();
        inline int get_num_tri() const {return (int)triangles.size();};
        inline int get_num_vert() const {return 3*(int)triangles.size();};
        Vec3 get_bbox();
        inline Vec3 get_bbox_Min_Corner() {return this->bboxMinCorner;};
        Vec3 posAt(const int p) const;
        Vec3 triangleAt(const int t) const;
    private:
        void read_stl_file(std::string fname);
        std::vector<Triangle> triangles;
        bool bboxComputed;
        Vec3 bboxMinCorner;
        Vec3 bboxMaxCorner;
        Vec3 bboxExtend;
};


#endif