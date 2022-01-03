/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_NORMALIZATION_H
#define NVL_MATH_NORMALIZATION_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class T>
void normalize(std::vector<T>& vector);
template<class T>
std::vector<T> normalized(const std::vector<T>& vector);

}

#include "normalization.cpp"

#endif // NVL_MATH_NORMALIZATION_H
