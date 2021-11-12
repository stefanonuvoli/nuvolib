#ifndef NVL_UTILITIES_TIMER_H
#define NVL_UTILITIES_TIMER_H

#include <nvl/nuvolib.h>

#include <iostream>
#include <chrono>
#include <string>

namespace nvl {

class Timer
{

public:

    Timer();
    Timer(const std::string& name);

    void reset();

    double elapsed() const;
    double elapsedSeconds() const;

    std::string elapsedString() const;
    void print() const;

private:

    typedef std::chrono::high_resolution_clock vClock;

    std::string vName;
    std::chrono::time_point<vClock> vStart;

    typedef std::chrono::duration<double, std::ratio<1,1000>> vMillisecondDuration;
    typedef std::chrono::duration<double, std::ratio<1>> vSecondDuration;

};

}

#include "timer.cpp"

#endif // NVL_UTILITIES_TIMER_H
