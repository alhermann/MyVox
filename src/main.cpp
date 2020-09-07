#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "Geometry.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Mat4.h"
#include "Array3D.h"
#include "Timer.h"
#include "AAB.h"
#include <cmath>
#include <typeinfo>
#include <thread>

// --------------------------------------------------------------

// Macros
#define VOXEL_RESOLUTION 128
#define VOXEL_FILL_INSIDE 1
#define VOXEL_ROBUST_FILL 1

// --------------------------------------------------------------

#define ALONG_X  1 
#define ALONG_Y  2
#define ALONG_Z  4 
#define INSIDE   8
#define INSIDE_X 16
#define INSIDE_Y 32
#define INSIDE_Z 64

// --------------------------------------------------------------

#define FP_POW 16
#define FP_SCALE (1<<FP_POW)
#define BOX_SCALE Vec3(VOXEL_RESOLUTION*FP_SCALE)

// --------------------------------------------------------------

#define EPS 10E-12

// --------------------------------------------------------------

#define SCALE_X 0
#define SCALE_Y 0
#define SCALE_Z 1

// --------------------------------------------------------------

Vec3 operator*(const Vec3& _Vec3, const double& rv) {
    return Vec3(_Vec3.x * rv, _Vec3.y * rv, _Vec3.z * rv);
}

Vec3 operator*(const double& rv, const Vec3& _Vec3) {
    return Vec3(_Vec3.x * rv, _Vec3.y * rv, _Vec3.z * rv);
} 

// --------------------------------------------------------------

const Vec3 clamp(const Vec3& _Vec3, const Vec3& _Vec3Min, const Vec3& _Vec3Max) {
    if ((std::round(_Vec3.x) > _Vec3Min.x) && (std::round(_Vec3.y) > _Vec3Min.y) && (std::round(_Vec3.z) > _Vec3Min.z)){
        if ((std::round(_Vec3.x) < _Vec3Max.x) && (std::round(_Vec3.y) < _Vec3Max.y) && (std::round(_Vec3.z) < _Vec3Max.z)){
            return Vec3(_Vec3);
        }
        else {
            return Vec3(_Vec3Max);
        }
    }
    else {
        return Vec3(_Vec3Min);
    }
}

// --------------------------------------------------------------

// saves a .csv voxel file
void saveAsCSV(std::string fname, const Array3D<unsigned char>& voxs, 
                double& xMax, double& yMax, double& zMax, 
                double& xMin, double& yMin, double& zMin, 
                double& xMaxVox, double& yMaxVox, double& zMaxVox,
                double& xMinVox, double& yMinVox, double& zMinVox){
    std::ofstream f;
    long sx = voxs.xsize(), sy = voxs.ysize(), sz = voxs.zsize();
    f.open(fname, std::ios::out | std::ios::trunc);
    f << "X," << "Y," << "Z" << std::endl;

    int auxOut = 0;
    for(int i = sx - 1; i >= 0; i--){
        for(int j = 0; j < sy; j++){
            for(int k = sz - 1; k >= 0; k--){
                unsigned char v = voxs.at(i, j, k); 
                unsigned char pal = v != 0 ? 1 : 0;

                if (v == INSIDE){
                    pal = 1;
                }
                if (pal == 1){
                    if ((std::abs(xMax - xMin) > EPS) && (std::abs(xMaxVox - xMinVox) > EPS) && (SCALE_X))
                        f << static_cast<double>(i)*std::abs(xMax - xMin)/std::abs(xMaxVox - xMinVox) << "," << static_cast<double>(j)*std::abs(xMax - xMin)/std::abs(xMaxVox - xMinVox) << "," << static_cast<double>(k)*std::abs(xMax - xMin)/std::abs(xMaxVox - xMinVox) << std::endl;
                    else if ((std::abs(yMax - yMin) > EPS) && (std::abs(yMaxVox - yMinVox) > EPS) && (SCALE_Y))
                        f << static_cast<double>(i)*std::abs(yMax - yMin)/std::abs(yMaxVox - yMinVox) << "," << static_cast<double>(j)*std::abs(yMax - yMin)/std::abs(yMaxVox - yMinVox) << "," << static_cast<double>(k)*std::abs(yMax - yMin)/std::abs(yMaxVox - yMinVox) << std::endl;
                    else if ((std::abs(zMax - zMin) > EPS) && (std::abs(zMaxVox - zMinVox) > EPS) && (SCALE_Z))
                        f << static_cast<double>(i)*std::abs(zMax - zMin)/std::abs(zMaxVox - zMinVox) << "," << static_cast<double>(j)*std::abs(zMax - zMin)/std::abs(zMaxVox - zMinVox) << "," << static_cast<double>(k)*std::abs(zMax - zMin)/std::abs(zMaxVox - zMinVox) << std::endl;
                    else {
                        if (auxOut != 1){
                            std::cout << "No scaling could be done!\n" << std::endl; 
                            auxOut = 1;
                        }
                        f << i << "," << j << "," << k << std::endl; 
                    }
                }
            }
        }
    } 
    f.close();
}

// --------------------------------------------------------------

class swizzle_xyz
{
public:
  inline Vec3 forward(const Vec3& v)  const { return v; }
  inline Vec3 backward(const Vec3& v) const { return v; }
  inline int along() const { return ALONG_Z; }
};

class swizzle_zxy
{
public:
  inline Vec3 forward(const Vec3& v)  const { return Vec3(v[2], v[0], v[1]); }
  inline Vec3 backward(const Vec3& v) const { return Vec3(v[1], v[2], v[0]); }
  inline unsigned char along() const { return ALONG_Y; }
};

class swizzle_yzx
{
public:
  inline Vec3 forward(const Vec3& v)  const { return Vec3(v[1], v[2], v[0]); }
  inline Vec3 backward(const Vec3& v) const { return Vec3(v[2], v[0], v[1]); }
  inline unsigned char along() const { return ALONG_X; }
};

// --------------------------------------------------------------

inline bool isInTriangle(int i, int j, Vec3& p0, Vec3& p1, Vec3& p2, int& _depth)
{
    Vec2 delta_p0 = Vec2(i, j) - Vec2(p0);
    Vec2 delta_p1 = Vec2(i, j) - Vec2(p1);
    Vec2 delta_p2 = Vec2(i, j) - Vec2(p2);
    Vec2 delta10 = Vec2(p1) - Vec2(p0);
    Vec2 delta21 = Vec2(p2) - Vec2(p1);
    Vec2 delta02 = Vec2(p0) - Vec2(p2);

    int64_t c0 = (int64_t)delta_p0[0] * (int64_t)delta10[1] - (int64_t)delta_p0[1] * (int64_t)delta10[0];
    int64_t c1 = (int64_t)delta_p1[0] * (int64_t)delta21[1] - (int64_t)delta_p1[1] * (int64_t)delta21[0];
    int64_t c2 = (int64_t)delta_p2[0] * (int64_t)delta02[1] - (int64_t)delta_p2[1] * (int64_t)delta02[0];
    bool inside = (c0 <= 0 && c1 <= 0 && c2 <= 0) || (c0 >= 0 && c1 >= 0 && c2 >= 0);
    
    if (inside) {
        int64_t area = c0 + c1 + c2;
        int64_t b0 = (c1 << 10) / area;
        int64_t b1 = (c2 << 10) / area;
        int64_t b2 = (1 << 10) - b0 - b1;
        _depth = ((b0 * (int)p0.round2int()[2] + b1 * (int)p1.round2int()[2] + b2 * (int)p2.round2int()[2]) >> 10);
    }
    return inside;
}
// --------------------------------------------------------------

template <class S>
void rasterize(
    const Vec3&                 tri,
    const std::vector<Vec3>&    pts,
    Array3D<unsigned char>      _voxs)
    {
        Vec3 _tri;
        const S swizzler;
        Vec3 tripts[3] = {
            swizzler.forward(pts[tri[0]]),//.round2int(),
            swizzler.forward(pts[tri[1]]),//.round2int(),
            swizzler.forward(pts[tri[2]])//.round2int()
        };

        // Check if triangle is valid
        Vec2 delta10 = Vec2(tripts[1]) - Vec2(tripts[0]);
        Vec2 delta21 = Vec2(tripts[2]) - Vec2(tripts[1]);
        Vec2 delta02 = Vec2(tripts[0]) - Vec2(tripts[2]);
        if (delta10 == Vec2(0)) return;
        if (delta21 == Vec2(0)) return;
        if (delta02 == Vec2(0)) return;
        if (delta02[0] * delta10[1] - delta02[1] * delta10[0] == 0) return;
 
        // Proceed
        AAB<2, int> pixbx;
        pixbx.addPoint(Vec2(tripts[0].round2int()) / (int)FP_SCALE);
        pixbx.addPoint(Vec2(tripts[1].round2int()) / (int)FP_SCALE);
        pixbx.addPoint(Vec2(tripts[2].round2int()) / (int)FP_SCALE);

        for (int j = pixbx.minCorner()[1]; j <= pixbx.maxCorner()[1]; j++){
            for (int i = pixbx.minCorner()[0]; i <= pixbx.maxCorner()[0]; i++){
                int depth;
                if (isInTriangle(
                    (i << FP_POW) + (1 << (FP_POW - 1)), // centered
                    (j << FP_POW) + (1 << (FP_POW - 1)), // centered
                    tripts[0], tripts[1], tripts[2], depth))
                {
                    Vec3 vx = swizzler.backward(Vec3(i, j, depth >> FP_POW));
                    // tag the voxel as occupied
                    // NOTE: voxels are likely to be hit multiple times (e.g. thin features)
                    //       we flip the bit every time a hit occurs in a voxel
                    _voxs.at(vx[0], vx[1], vx[2]) = ( _voxs.at(vx[0], vx[1], vx[2]) ^ swizzler.along() );
                }
            }
        }
    }
 
 // --------------------------------------------------------------

// This version is more robust by using all three direction
// and voting among them to decide what is filled or not
void fillInsideVoting(Array3D<unsigned char>& _voxs)
{
  // along x
  for(int k = 0; k < (int)_voxs.zsize(); k++){
    for(int j = 0; j < (int)_voxs.ysize(); j++){
      bool inside = false;
      for(int i = 0; i < (int)_voxs.xsize(); i++){
        if (_voxs.at((unsigned int)i, (unsigned int)j, (unsigned int)k) & ALONG_X) {
          inside = !inside;
        }
        if (inside) {
          _voxs.at((unsigned int)i, (unsigned int)j, (unsigned int)k) |= INSIDE_X;
        }
      }
    }
  }
  // along y
  for(int k = 0; k < (int)_voxs.zsize(); k++){
    for(int j = 0; j < (int)_voxs.xsize(); j++){
      bool inside = false;
      for(int i = 0; i < (int)_voxs.ysize(); i++){
        if (_voxs.at((unsigned int)j, (unsigned int)i, (unsigned int)k) & ALONG_Y) {
          inside = !inside;
        }
        if (inside) {
          _voxs.at((unsigned int)j, (unsigned int)i, (unsigned int)k) |= INSIDE_Y;
        }
      }
    }
  }
  // along z
  for(int k = 0; k < (int)_voxs.zsize(); k++){
    for(int j = 0; j < (int)_voxs.xsize(); j++){
      bool inside = false;
      for(int i = 0; i < (int)_voxs.ysize(); i++){
        if (_voxs.at((unsigned int)j, (unsigned int)k, (unsigned int)i) & ALONG_Z) {
          inside = !inside;
        }
        if (inside) {
          _voxs.at((unsigned int)j, (unsigned int)k, (unsigned int)i) |= INSIDE_Z;
        }
      }
    }
  }
  // voting
 for(int k = 0; k < (int)_voxs.zsize(); k++){
        for(int j = 0; j < (int)_voxs.ysize(); j++){
            for (int i = 0; i < (int)_voxs.xsize(); i++){
                unsigned char v = _voxs.at((unsigned int)i, (unsigned int)j, (unsigned int)k);
                int votes =
                (  (v & INSIDE_X) ? 1 : 0)
                + ((v & INSIDE_Y) ? 1 : 0)
                + ((v & INSIDE_Z) ? 1 : 0);
                // clean
                _voxs.at((unsigned int)i, (unsigned int)j, (unsigned int)k) &= ~(INSIDE_X | INSIDE_Y | INSIDE_Z);
                if (votes > 1) {
                // tag as inside
                _voxs.at((unsigned int)i, (unsigned int)j, (unsigned int)k) |= INSIDE;
                }
            }
        }
    }
}


// --------------------------------------------------------------

void fillInside(Array3D<unsigned char>& _voxs){
    for(int k = 0; k < _voxs.zsize(); k++){
        for(int j = 0; j < _voxs.ysize(); j++){
            bool inside = false;
            for(int i = 0; i < _voxs.xsize(); i++){
                if (_voxs.at((unsigned int)i, (unsigned int)j, (unsigned int)k) & ALONG_X){
                    inside = !inside;
                }
                if (inside){
                    _voxs.at((unsigned int)i, (unsigned int)j, (unsigned int)k) |= INSIDE;
                }
            }
        }
    }
}
 
// --------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Input 
    if (argc != 2){
        std::cerr << "Usage: ./MyVox <path to file>.stl" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    try{
    std::string fname(argv[1]);

    // Import triangular geometry
    Geometry geo(fname);
    // Produce (fixed fp) integer vertices and triangles
    std::vector<Vec3> pts;
    std::vector<Vec3> tris;

    double xMax = 0.0;
    double yMax = 0.0;
    double zMax = 0.0;
    double xMin = 1e20f;
    double yMin = 1e20f;
    double zMin = 1e20f;
    double xMaxVox = 0.0;
    double yMaxVox = 0.0;
    double zMaxVox = 0.0;
    double xMinVox = 1e20f;
    double yMinVox = 1e20f;
    double zMinVox = 1e20f;
    {
        // tmp variables for same floating point precision
        double factor = 0.95f;
        double bbMax = std::max(std::max(geo.get_bbox().x,geo.get_bbox().y),geo.get_bbox().z);
        Vec3 tmp1 = (Vec3(1.f) / bbMax);
        Vec3 tmp2___ = Vec3(double(geo.get_bbox().x), double(geo.get_bbox().y), double(geo.get_bbox().z));
        Vec3 tmp2__ = 0.5f * tmp2___;
        Vec3 tmp2_ = (1 - factor) * tmp2__;
        Vec3 tmp2 = Vec3(double(tmp2_.x), double(tmp2_.y), double(tmp2_.z));
        Vec3 tmp3 = Vec3(factor);
        Vec3 tmp4 = -1 * geo.get_bbox_Min_Corner();
        Matrix4d<> boxtrsf = scaleMatrix(BOX_SCALE) 
                            * scaleMatrix(Vec3(double(tmp1.x), double(tmp1.y), double(tmp1.z))) 
                            * translationMatrix(Vec3(double(tmp2.x), double(tmp2.y), double(tmp2.z)))
                            * scaleMatrix(Vec3(double(tmp3.x), double(tmp3.y), double(tmp3.z)))
                            * translationMatrix(Vec3(double(tmp4.x), double(tmp4.y), double(tmp4.z)));
                            
        // transform vertices
        pts.resize(geo.get_num_vert());
        for (int p = 0; p < geo.get_num_vert(); p++){
            Vec3 pt = geo.posAt(p); 
            Vec3 bxpt = boxtrsf.mulPoint(pt);
            Vec3 ipt = clamp(bxpt.round2int(), Vec3(0.0f), BOX_SCALE - Vec3(1.0f));
            pts[p] = ipt;
        }

        for (int p = 0; p < geo.get_num_vert(); p++){
            if ((geo.posAt(p)(0)) > xMax)
                xMax = static_cast<double>(geo.posAt(p)(0));
            if ((geo.posAt(p)(1)) > yMax)
                yMax = static_cast<double>(geo.posAt(p)(1));
            if ((geo.posAt(p)(2)) > zMax)
                zMax = static_cast<double>(geo.posAt(p)(2));

            if ((geo.posAt(p)(0)) < xMin)
                xMin = static_cast<double>(geo.posAt(p)(0));
            if ((geo.posAt(p)(1)) < yMin)
                yMin = static_cast<double>(geo.posAt(p)(1));
            if ((geo.posAt(p)(2)) < zMin)
                zMin = static_cast<double>(geo.posAt(p)(2));
        }

        // prepare triangles
        tris.reserve(geo.get_num_tri());
        for (int t = 0; t < geo.get_num_tri(); t++){
            Vec3 tri = geo.triangleAt(t);
            tris.push_back(tri);
        }
    } 

    // rasterize into voxels
    Vec3 resolution(geo.get_bbox() / geo.get_bbox().tupleMax() * float(VOXEL_RESOLUTION));
    resolution.cast_uint();

    Array3D<unsigned char> voxs(resolution);
    voxs.fill(0);
    {
        std::string s = "Rasterization";
        Timer tm(s);

        for (int t = 0; t < tris.size(); t++){
            rasterize<swizzle_xyz>(tris[t], pts, voxs); // xy view
            rasterize<swizzle_yzx>(tris[t], pts, voxs); // yz view
            rasterize<swizzle_zxy>(tris[t], pts, voxs); // zx view
        }
        std::cout << std::endl;
    }

    // add inner voxels
#if VOXEL_FILL_INSIDE
    {
        Timer tm("Fill");
        std::cout << "Filling in/out ... ";

        #if VOXEL_ROBUST_FILL
            fillInsideVoting(voxs);
        #else
            fillInside(voxs);
        #endif

        std::cout << " done." << std::endl;
    }
#endif

    for(int k = 0; k < (int)voxs.zsize(); k++){
        for(int j = 0; j < (int)voxs.ysize(); j++){
            for (int i = 0; i < (int)voxs.xsize(); i++){
                if (voxs.at(i, j, k) > 0) {
                    if (static_cast<double>(i) > xMaxVox)
                        xMaxVox = static_cast<double>(i);
                    if (static_cast<double>(j) > yMaxVox)
                        yMaxVox = static_cast<double>(j);
                    if (static_cast<double>(k) > zMaxVox)
                        zMaxVox = static_cast<double>(k);

                    if (static_cast<double>(i) < xMinVox)
                        xMinVox = static_cast<double>(i);
                    if (static_cast<double>(j) < yMinVox)
                        yMinVox = static_cast<double>(j);
                    if (static_cast<double>(k) < zMinVox)
                        zMinVox = static_cast<double>(k);
                }
            }
        }
    }

    // save the result
    saveAsCSV("out.csv", voxs, xMax, yMax, zMax, xMin, yMin, zMin, xMaxVox, yMaxVox, zMaxVox, xMinVox, yMinVox, zMinVox);

    // report some stats
    int num_in_vox = 0;
    for(int k = 0; k < (int)voxs.zsize(); k++){
        for(int j = 0; j < (int)voxs.ysize(); j++){
            for (int i = 0; i < (int)voxs.xsize(); i++){
                if (voxs.at(i, j, k) > 0) {
                    num_in_vox++;
                }     
            }
        }
    }

    std::cout << "Number of set voxels: " << num_in_vox << std::endl;

    return 0; 
    }
    catch(std::exception &exc) {
        std::cerr << "An error occured: " << std::endl << exc.what() 
        << std::endl << "Aborting!" << std::endl;
        return 1;
    }
}    