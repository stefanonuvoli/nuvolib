/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_UTILITIES_TIMER_H
#define NVL_UTILITIES_TIMER_H

#include <nvl/nuvolib.h>

#include <iostream>
#include <chrono>
#include <string>

namespace nvl {

/**
 * @brief A timer which allows to measure the time passed between two moments.
 */
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
