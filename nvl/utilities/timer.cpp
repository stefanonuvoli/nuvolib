/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "timer.h"

#include <sstream>
#include <iomanip>

namespace nvl {

/**
 * @brief Default constructor
 */
Timer::Timer() : Timer("")
{

}

/**
 * @brief Constructor with name
 * @param name Name of the timer
 */
Timer::Timer(const std::string& name) : vName(name), vStart(vClock::now())
{

}

/**
 * @brief Reset the timer.
 */
void Timer::reset()
{
    vStart = vClock::now();
}

/**
 * @brief Get the time elapsed since the reset of the timer
 * @return Time elapsed
 */
double Timer::elapsed() const
{
    return std::chrono::duration_cast<vMillisecondDuration>(vClock::now() - vStart).count();
}

/**
 * @brief Get the second elapsed since the reset of the timer
 * @return Time elapsed
 */
double Timer::elapsedSeconds() const
{
    return std::chrono::duration_cast<vSecondDuration>(vClock::now() - vStart).count();
}

/**
 * @brief Get the time elapsed since the reset of the timer
 * @return Time elapsed
 */
std::string Timer::elapsedString() const {
    std::stringstream stream;

    if (!vName.empty())
        stream << vName << ": ";

    stream << elapsed() << " ms";

    return stream.str();
}

/**
 * @brief Print the elapsed time
 */
void Timer::print() const {
    std::cout << elapsedString() << std::endl;
}

}
