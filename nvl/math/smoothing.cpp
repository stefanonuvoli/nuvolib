#include "smoothing.h"

#include <nvl/math/comparisons.h>

namespace nvl {

template<class T>
void laplacianSmoothing(
        std::vector<T>& vector,
        const std::vector<std::vector<Index>>& adjacencies,
        const Index iterations,
        const double alpha)
{
    assert(alpha >= 0.0 && alpha <= 1.0);

    if (nvl::epsEqual(alpha, 1.0))
        return;

    for (Index it = 0; it < iterations; ++it) {
        std::vector<T> tmpVector = vector;

        #pragma omp parallel for
        for (Index i = 0; i < tmpVector.size(); ++i) {
            T value = alpha * tmpVector[i];
            double adjAlpha = (1 - alpha) / adjacencies[i].size();
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
        const Index iterations,
        const std::vector<double>& alphas)
{
    for (Index it = 0; it < iterations; ++it) {
        std::vector<T> tmpVector = vector;

        #pragma omp parallel for
        for (Index i = 0; i < tmpVector.size(); ++i) {
            assert(alphas[i] >= 0.0 && alphas[i] <= 1.0);

            if (!nvl::epsEqual(alphas[i], 1.0)) {
                T value = alphas[i] * tmpVector[i];
                double adjAlpha = (1 - alphas[i]) / adjacencies[i].size();
                for (const Index& adj : adjacencies[i]) {
                    value += adjAlpha * tmpVector[adj];
                }
                vector[i] = value;
            }
        }
    }
}

}
