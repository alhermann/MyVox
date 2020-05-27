#ifndef TIMER_H
#define TIMER_H

#include "sys/time.h"
#include <iostream>
#include <iomanip>

typedef long long t_time;

class Timer{
    protected:
        const std::string m_Name;
        bool m_Active;
        t_time m_TmAccum;
        t_time m_TmStart;
    public:
        Timer(const std::string name="[Timer] ", bool autostart=true);
        ~Timer();
        void start();
        t_time stop();
        t_time elapsed();
        void display();
        inline t_time milliseconds(){
            struct timeval now;
            static struct timeval start; 

            static bool init = false;

            if (!init) {
                gettimeofday(&start, NULL);
                init = true;
            }

            gettimeofday(&now, NULL);
            int ms = int((now.tv_sec-start.tv_sec)*1000+(now.tv_usec-start.tv_usec)/1000);
            return ms;
        };
};

#endif