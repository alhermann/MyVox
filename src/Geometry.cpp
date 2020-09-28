#include <fstream>
#include <cmath>
#include <iostream>
#include "Geometry.h"

Geometry::Geometry(std::string fname) : bboxComputed(false), bboxMinCorner(0.f), bboxMaxCorner(0.f), bboxExtend(0.f), cog(0.f), totalVolume(0.f) {
    std::cout << "Reading geometry ...\n";
    Geometry::read_stl_file(fname);
    std::cout << "done." << std::endl;
    std::cout << "Number of triangles: " << Geometry::get_num_tri() << std::endl;
}

void Geometry::read_stl_file(std::string fname){
    std::ifstream stlFile(fname.c_str(), std::ios::in | std::ios::binary);
    char headInfo[80] = "";
    // Read 80 byte header
    if (stlFile){
        stlFile.read(headInfo, 80);
        std::cout << "STL file header comment: " << headInfo << std::endl;
        char nTriRaw[4];
        stlFile.read(nTriRaw, 4);
        unsigned numTri = *((unsigned*)nTriRaw);
        triangles.resize(numTri);
        int i = 0;
        for (auto&& tri : triangles){
            char triRaw[50];
            stlFile.read(triRaw, 50);
            Vec3 norm(triRaw);
            Vec3 p1(triRaw+12);
            Vec3 p2(triRaw+24);
            Vec3 p3(triRaw+36);
            double idx1(i);
            double idx2(i+1);
            double idx3(i+2);
            tri = Triangle(norm, p1, p2, p3, idx1, idx2, idx3);
            i += 3;
        }
    }
    else {
        std::cerr << "An error occured while opening the STL file." << std::endl;
    }
}

Geometry::~Geometry(){}

Vec3 Geometry::get_bbox() {
    if (!this->bboxComputed)
    {
        double minx = 1e20f, maxx = -1e20f;
        double miny = 1e20f, maxy = -1e20f;
        double minz = 1e20f, maxz = -1e20f;

	double totalVolume = 0.0, currentVolume;
	double xCenter = 0.0, yCenter = 0.0, zCenter = 0.0;

        for (int i = 0; i < this->get_num_tri(); i++)
        {   for (int j = 0; j < 3; j++)
            {
                double x,y,z;
                if (j == 0)
                {
                    x = triangles[i].p1.x;    
                    y = triangles[i].p1.y;
                    z = triangles[i].p1.z;
                }
                else if (j == 1)
                {
                    x = triangles[i].p2.x;    
                    y = triangles[i].p2.y;
                    z = triangles[i].p2.z;
                }
                else 
                {
                    x = triangles[i].p3.x;    
                    y = triangles[i].p3.y;
                    z = triangles[i].p3.z;
                }
                if (x > maxx) maxx = x;
                if (x < minx) minx = x;
                if (y > maxy) maxy = y;
                if (y < miny) miny = y;
                if (z > maxz) maxz = z;
                if (z < minz) minz = z;
            } 
		totalVolume += currentVolume = (triangles[i].p1.x*triangles[i].p2.y*triangles[i].p3.z - triangles[i].p1.x*triangles[i].p3.y*triangles[i].p2.z - triangles[i].p2.x*triangles[i].p1.y*triangles[i].p3.z + triangles[i].p2.x*triangles[i].p3.y*triangles[i].p1.z + triangles[i].p3.x*triangles[i].p1.y*triangles[i].p2.z - triangles[i].p3.x*triangles[i].p2.y*triangles[i].p1.z) / 6.;
		xCenter += ((triangles[i].p1.x + triangles[i].p2.x + triangles[i].p3.x) / 4.) * currentVolume;
		yCenter += ((triangles[i].p1.y + triangles[i].p2.y + triangles[i].p3.y) / 4.) * currentVolume;
		zCenter += ((triangles[i].p1.z + triangles[i].p2.z + triangles[i].p3.z) / 4.) * currentVolume;
        }
        this->bboxMinCorner.x = minx;
        this->bboxMinCorner.y = miny;
        this->bboxMinCorner.z = minz;
        this->bboxMaxCorner.x = maxx;
        this->bboxMaxCorner.y = maxy; 
        this->bboxMaxCorner.z = maxz;
        this->bboxExtend.x = this->bboxMaxCorner.x - this->bboxMinCorner.x;
        this->bboxExtend.y = this->bboxMaxCorner.y - this->bboxMinCorner.y;
        this->bboxExtend.z = this->bboxMaxCorner.z - this->bboxMinCorner.z;
        this->bboxComputed = true;
    	this->cog.x = xCenter/totalVolume;
    	this->cog.y = yCenter/totalVolume;
    	this->cog.z = zCenter/totalVolume;
    	this->totalVolume = totalVolume;    
	}
    return this->bboxExtend;
}

Vec3 Geometry::posAt(const int p) const{
    int indx = (static_cast<int>(std::ceil( (p + 1) /3.0)) - 1);
    if (((p+1) % 3) == 1) {
            return triangles[indx].p1;
    }
    else if (((p+1) % 3) == 2) {
            return triangles[indx].p2;
    }            
    else if (((p+1) % 3) == 0) {
            return triangles[indx].p3;
    }
    else {
        std::cerr << "An error occured. Out of bounds." << std::endl;
        return 1;
    }
}

Vec3 Geometry::triangleAt(const int t) const{
    if (t >= 0) {
        return Vec3(triangles[t].idx1, triangles[t].idx2, triangles[t].idx3);
    }
    else {
        std::cerr << "An error occured. Out of bounds." << std::endl;
        return 1;
    }
}
