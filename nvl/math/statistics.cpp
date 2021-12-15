#include "statistics.h"

#include <nvl/math/numeric_limits.h>
#include <nvl/math/common_functions.h>
#include <nvl/math/normalization.h>

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

template<class T, class R>
std::vector<R> frequencyBins(const std::vector<T>& values, const unsigned int nBins)
{
    return frequencyBins(values, min(values), max(values), nBins);
}

template<class T, class R>
std::vector<R> frequencyBins(const std::vector<T>& values, const T& minValue, const T& maxValue, const unsigned int nBins)
{
    std::vector<R> bins;

    if (!values.empty() && nBins > 0) {
        bins.resize(nBins, 0);

        const T step = (maxValue - minValue) / static_cast<R>(nBins);

        for (const T& value : values) {
            unsigned int bin = static_cast<unsigned int>(value / step);
            bins[bin]++;
        }
    }

    return bins;
}


template<class T, class R>
std::vector<R> sampleUniformlyDistributedScore(const std::vector<T>& values, const unsigned int nBins)
{
    return frequencyBins(values, min(values), max(values), nBins);
}

template<class T, class R>
R sampleUniformlyDistributedScore(const std::vector<T>& values, const T& minValue, const T& maxValue, const unsigned int nBins)
{
    std::vector<unsigned int> bins = frequencyBins(values, minValue, maxValue, nBins);

    std::vector<R> binsP(bins.size());
    for (Index i = 0; i < binsP.size(); ++i) {
        binsP[i] = static_cast<R>(bins[i]);
    }
    normalize(binsP);

    R expectedP = static_cast<R>(1.0) / static_cast<R>(binsP.size());

    R cs = 0;
    for (Index i = 0; i < binsP.size(); ++i) {
        R diff = binsP[i] - expectedP;
        cs += (diff * diff) / expectedP;
    }
    cs = sqrt(cs);

    cs /= binsP.size();

    assert(cs >= 0 - EPSILON && cs <= 1 + EPSILON);

    cs = max(min(cs, 1.0), 0.0);

    return 1.0 - cs;
}

}
