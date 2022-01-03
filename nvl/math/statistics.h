/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_STATISTICS_H
#define NVL_MATH_STATISTICS_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class T, class R = T>
R mean(const std::vector<T>& values);

template<class T, class R = T>
R variance(const std::vector<T>& values);
template<class T, class R = T>
R variance(const std::vector<T>& values, const T& mean);

template<class T, class R = T>
R stddev(const std::vector<T>& values);
template<class T, class R = T>
R stddev(const std::vector<T>& values, const T& mean);

template<class T, class R = unsigned int>
std::vector<R> frequencyBins(const std::vector<T>& values, const unsigned int nBins);
template<class T, class R = unsigned int>
std::vector<R> frequencyBins(const std::vector<T>& values, const T& minValue, const T& maxValue, const unsigned int nBins);

template<class T, class R = double>
R sampleUniformlyDistributedScore(const std::vector<T>& values, const unsigned int nBins);
template<class T, class R = double>
R sampleUniformlyDistributedScore(const std::vector<T>& values, const T& minValue, const T& maxValue, const unsigned int nBins);

}

#include "statistics.cpp"

#endif // NVL_MATH_STATISTICS_H
