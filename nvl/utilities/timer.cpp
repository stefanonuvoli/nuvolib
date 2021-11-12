#include "timer.h"

#include <sstream>
#include <iomanip>

namespace nvl {

Timer::Timer() : Timer("")
{

}

Timer::Timer(const std::string &name) : vName(name), vStart(vClock::now())
{

}

void Timer::reset()
{
    vStart = vClock::now();
}

double Timer::elapsed() const
{
    return std::chrono::duration_cast<vMillisecondDuration>(vClock::now() - vStart).count();
}

double Timer::elapsedSeconds() const
{
    return std::chrono::duration_cast<vSecondDuration>(vClock::now() - vStart).count();
}

std::string Timer::elapsedString() const {
    //TODO HOURS/MINUTES/SECONDS

    std::stringstream stream;

    if (!vName.empty())
        stream << vName << ": ";

    stream << elapsed() << " ms";

    return stream.str();
}

void Timer::print() const {
    std::cout << elapsedString() << std::endl;
}

}
