/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "smoothing.h"

#include <nvl/math/comparisons.h>

namespace nvl {

template<class T>
void laplacianSmoothing(
        std::vector<T>& vector,
        const std::vector<std::vector<Index>>& adjacencies,
        const unsigned int iterations,
        const double alpha)
{
    assert(alpha >= 0.0 && alpha <= 1.0);

    if (epsEqual(alpha, 1.0))
        return;

    for (unsigned int it = 0; it < iterations; ++it) {
        std::vector<T> tmpVector = vector;

        #pragma omp parallel for
        for (Index i = 0; i < tmpVector.size(); ++i) {
            T value = alpha * tmpVector[i];
            double adjAlpha = (1.0 - alpha) / adjacencies[i].size();
            for (const Index& adj : adjacencies[i]) {
                value += adjAlpha * tmpVector[adj];
            }
            vector[i] = value;
        }
    }
}

template<class T>
void laplacianSmoothing(
        std::vector<T>& vector,
        const std::vector<std::vector<Index>>& adjacencies,
        const unsigned int iterations,
        const std::vector<double>& alphas)
{
    for (unsigned int it = 0; it < iterations; ++it) {
        std::vector<T> tmpVector = vector;

        #pragma omp parallel for
        for (Index i = 0; i < tmpVector.size(); ++i) {
            assert(alphas[i] >= 0.0 && alphas[i] <= 1.0);

            if (!epsEqual(alphas[i], 1.0)) {
                T value = alphas[i] * tmpVector[i];
                double adjAlpha = (1.0 - alphas[i]) / adjacencies[i].size();
                for (const Index& adj : adjacencies[i]) {
                    value += adjAlpha * tmpVector[adj];
                }
                vector[i] = value;
            }
        }
    }
}

}
