#ifndef AAB_H
#define AAB_H

#include <iostream>
#include <limits>
#include "Vec2.h"

// Axis Aligned Box

template <int T_NumDim, typename T_Type = double>
class AAB
{
    typedef Vec2 t_Vertex;

    public:
        t_Vertex m_Mins;
        t_Vertex m_Maxs;

        AAB(){
            m_Mins = std::numeric_limits<T_Type>::max();
            m_Maxs = -std::numeric_limits<T_Type>::max();
        }

        AAB(const AAB& aab){
            this->m_Mins = aab.m_Mins;
            this->m_Maxs = aab.m_Maxs;
        }

        AAB(const t_Vertex& mins, const t_Vertex& maxs){
            this->m_Mins = mins;
            this->m_Maxs = maxs;
        }

        void addPoint(const t_Vertex& p){
            this->m_Mins = p.Minof2(this->m_Mins);
            this->m_Maxs = p.Maxof2(this->m_Maxs);
        }

        t_Vertex& minCorner() {return this->m_Mins;}
        t_Vertex& maxCorner() {return this->m_Maxs;}
};

#endif