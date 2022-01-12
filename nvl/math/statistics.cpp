/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "statistics.h"

#include <nvl/math/numeric_limits.h>
#include <nvl/math/common_functions.h>
#include <nvl/math/normalization.h>

namespace nvl {

/**
 * @brief Mean (average) of a collection of values
 * @param values Values
 * @return Mean
 */
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

/**
 * @brief Variance of a collection of values
 * @param values Values
 * @return Variance
 */
template<class T, class R>
R variance(const std::vector<T>& values)
{
    return variance(values, mean(values));
}

/**
 * @brief Variance of a collection of values
 * @param values Values
 * @param mean Mean of the values
 * @return Variance
 */
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

/**
 * @brief Standard deviation of a collection of values
 * @param values Values
 * @return Standard deviation
 */
template<class T, class R>
R stddev(const std::vector<T>& values)
{
    return stddev(values, mean(values));
}

/**
 * @brief Standard deviation of a collection of values
 * @param values Values
 * @param mean Mean of the values
 * @return Standard deviation
 */
template<class T, class R>
R stddev(const std::vector<T>& values, const T& mean)
{
    return sqrt(variance(values, mean));
}

/**
 * @brief Calculate frequency bins of a collection of values
 * @param values Values
 * @param nBins Number of bins
 * @return Frequency bins
 */
template<class T, class R>
std::vector<R> frequencyBins(const std::vector<T>& values, const unsigned int nBins)
{
    return frequencyBins(values, min(values), max(values), nBins);
}

/**
 * @brief Calculate frequency bins of a collection of values
 * @param values Values
 * @param minValue Min value
 * @param maxValue Max value
 * @param nBins Number of bins
 * @return Frequency bins
 */
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

/**
 * @brief Get a score (from 0 to 1) of how much a collection of values is uniformly distributed
 * @param values Values
 * @param nBins Number of bins
 * @return score
 */
template<class T, class R>
std::vector<R> sampleUniformlyDistributedScore(const std::vector<T>& values, const unsigned int nBins)
{
    return frequencyBins(values, min(values), max(values), nBins);
}

/**
 * @brief Get a score (from 0 to 1) of how much a collection of values is uniformly distributed
 * @param values Values
 * @param minValue Min value
 * @param maxValue Max value
 * @param nBins Number of bins
 * @return score
 */
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
