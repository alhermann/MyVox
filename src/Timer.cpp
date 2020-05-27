#include "Timer.h"

Timer::Timer(const std::string name, bool autostart) : m_Name(name), m_Active(false){
    if (autostart){
        start();
    }
}

Timer::~Timer(){
    stop();
    display();
}

void Timer::start(){
    m_TmStart = milliseconds();
    m_Active = true;
}

t_time Timer::stop(){
    if (m_Active) {
        t_time tm = elapsed();
        m_TmAccum += tm;
        m_Active = false;
        return tm;
    }
    else {
        return 0;
    }
}

t_time Timer::elapsed(){
    return (milliseconds() - m_TmStart);
}

void Timer::display(){
    t_time h = (m_TmAccum / (1000 * 60 * 60));
    t_time m = ((m_TmAccum / (1000 * 60)) % 60);
    t_time s = ((m_TmAccum / 1000) % 60);
    t_time ms = m_TmAccum % 1000;

    std::cout << m_Name << "s: " << std::setprecision(2) << (int)h << " hours " << (int)m << " min " << (int)s << " s " << std::setprecision(4) << (int)ms << " ms\n" << std::endl;
}
