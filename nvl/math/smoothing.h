/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_SMOOTHING_H
#define NVL_MATH_SMOOTHING_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class T>
void laplacianSmoothing(
        std::vector<T>& vector,
        const std::vector<std::vector<Index>>& adjacencies,
        const unsigned int iterations,
        const double alpha);

template<class T>
void laplacianSmoothing(
        std::vector<T>& vector,
        const std::vector<std::vector<Index>>& adjacencies,
        const unsigned int iterations,
        const std::vector<double>& alphas);

template<class T>
void laplacianSmoothing(
        std::vector<T>& line,
        const unsigned int iterations,
        const double alpha);

template<class T>
void laplacianSmoothing(
        std::vector<T>& line,
        const unsigned int iterations,
        const std::vector<double>& alphas);

}

#include "smoothing.cpp"

#endif // NVL_MATH_SMOOTHING_H
