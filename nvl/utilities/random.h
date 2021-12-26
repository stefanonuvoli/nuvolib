/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_UTILITIES_RANDOM_H
#define NVL_UTILITIES_RANDOM_H

#include <nvl/nuvolib.h>

#include <limits>

namespace nvl {

template<class T>
T getRandom(T min = std::numeric_limits<T>::lowest(), T max = std::numeric_limits<T>::max());

double getRandomDouble(double min = std::numeric_limits<double>::lowest(), double max = std::numeric_limits<double>::max());
float getRandomFloat(float min = std::numeric_limits<float>::lowest(), float max = std::numeric_limits<float>::max());

int getRandomInt(int min = std::numeric_limits<int>::lowest(), int max = std::numeric_limits<int>::max());
long long int getRandomLongLongInt(long long int min = std::numeric_limits<long long int>::lowest(), long long int max = std::numeric_limits<long long int>::max());

unsigned int getRandomUnsignedInt(unsigned int min = std::numeric_limits<unsigned int>::lowest(), unsigned int max = std::numeric_limits<unsigned int>::max());
long long unsigned int getRandomLongLongUnsignedInt(long long unsigned int min = std::numeric_limits<long long unsigned int>::lowest(), long long unsigned int max = std::numeric_limits<long long int>::max());

}

#include "random.cpp"

#endif // NVL_UTILITIES_RANDOM_H
