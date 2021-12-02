#ifndef NVL_MATH_STATISTICS_H
#define NVL_MATH_STATISTICS_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class T, class R = double>
R mean(const std::vector<T>& values);

template<class T, class R = double>
R variance(const std::vector<T>& values);
template<class T, class R = double>
R variance(const std::vector<T>& values, const T& mean);

template<class T, class R = double>
R stddev(const std::vector<T>& values);
template<class T, class R = double>
R stddev(const std::vector<T>& values, const T& mean);


}

#include "statistics.cpp"

#endif // NVL_MATH_STATISTICS_H
