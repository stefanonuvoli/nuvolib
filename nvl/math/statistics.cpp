#include "statistics.h"

#include <nvl/math/numeric_limits.h>
#include <nvl/math/common_functions.h>

namespace nvl {

template<class T, class R>
R mean(const std::vector<T>& values)
{
    if (values.empty())
        return maxLimitValue<T>();

    T sum = values[0];
    for (Index i = 1; i < values.size(); ++i)
        sum += values[i];

    R avg = static_cast<R>(sum) / static_cast<R>(values.size());
    return avg;
}

template<class T, class R>
R variance(const std::vector<T>& values)
{
    return variance(values, mean(values));
}

template<class T, class R>
R variance(const std::vector<T>& values, const T& mean)
{
    if (values.empty())
        return maxLimitValue<T>();

    T sum = values[0] * values[0];
    for (Index i = 1; i < values.size(); ++i) {
        sum += values[i] * values[i];
    }

    R var = (static_cast<R>(sum) / static_cast<R>(values.size())) - static_cast<R>(mean * mean);

    return var;
}

template<class T, class R>
R stddev(const std::vector<T>& values)
{
    return stddev(values, mean(values));
}
template<class T, class R>
R stddev(const std::vector<T>& values, const T& mean)
{
    return sqrt(variance(values, mean));
}

}
