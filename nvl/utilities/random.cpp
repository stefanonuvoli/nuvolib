#include "random.h"

#include <random>

namespace nvl {

template<class T>
T getRandom(T min, T max)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(static_cast<double>(min), static_cast<double>(max));

    double randomValue = dist(rng);

    return static_cast<T>(randomValue);
}

NVL_INLINE double getRandomDouble(double min, double max)
{
    return getRandom(min, max);
}

NVL_INLINE float getRandomFloat(float min, float max)
{
    return getRandom(min, max);
}

NVL_INLINE int getRandomInt(int min, int max)
{
    return getRandom(min, max);
}

NVL_INLINE long long getRandomLongLongInt(long long min, long long max)
{
    return getRandom(min, max);
}

NVL_INLINE unsigned int getRandomUnsignedInt(unsigned int min, unsigned int max)
{
    return getRandom(min, max);
}

NVL_INLINE unsigned long long getRandomLongLongUnsignedInt(unsigned long long min, unsigned long long max)
{
    return getRandom(min, max);
}

}
