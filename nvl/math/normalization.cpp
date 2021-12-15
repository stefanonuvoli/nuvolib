#include "normalization.h"

#include <nvl/math/comparisons.h>

namespace nvl {

template<class T>
void normalize(std::vector<T>& vector)
{
    T sum = 0;

    for (const T& v : vector) {
        sum += v;
    }

    if (!epsEqual(sum, 0)) {
        for (T& v : vector) {
            v /= sum;
        }
    }
}

template<class T>
std::vector<T> normalized(const std::vector<T>& vector)
{
    std::vector<T> normalizedVector = vector;

    normalize(normalizedVector);

    return normalizedVector;
}

}
