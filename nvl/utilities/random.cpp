/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "random.h"

#include <random>

namespace nvl {

/**
 * @brief Random generation of a value
 * @param min Min value
 * @param max Max value
 * @return Random value
 */
template<class T>
T getRandom(T min, T max)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(static_cast<double>(min), static_cast<double>(max));

    double randomValue = dist(rng);

    return static_cast<T>(randomValue);
}

/**
 * @brief Random generation of a double value
 * @param min Min value
 * @param max Max value
 * @return Random value
 */
NVL_INLINE double getRandomDouble(double min, double max)
{
    return getRandom(min, max);
}

/**
 * @brief Random generation of a float value
 * @param min Min value
 * @param max Max value
 * @return Random value
 */
NVL_INLINE float getRandomFloat(float min, float max)
{
    return getRandom(min, max);
}

/**
 * @brief Random generation of a integer value
 * @param min Min value
 * @param max Max value
 * @return Random value
 */
NVL_INLINE int getRandomInt(int min, int max)
{
    return getRandom(min, max);
}

/**
 * @brief Random generation of a long long integer value
 * @param min Min value
 * @param max Max value
 * @return Random value
 */
NVL_INLINE long long getRandomLongLongInt(long long min, long long max)
{
    return getRandom(min, max);
}

/**
 * @brief Random generation of a unsigned integer value
 * @param min Min value
 * @param max Max value
 * @return Random value
 */
NVL_INLINE unsigned int getRandomUnsignedInt(unsigned int min, unsigned int max)
{
    return getRandom(min, max);
}

/**
 * @brief Random generation of a long long unsigned integer value
 * @param min Min value
 * @param max Max value
 * @return Random value
 */
NVL_INLINE unsigned long long getRandomLongLongUnsignedInt(unsigned long long min, unsigned long long max)
{
    return getRandom(min, max);
}

}
