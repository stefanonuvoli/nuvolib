/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "animation_smoothing.h"

#include <nvl/math/interpolation.h>
#include <nvl/models/algorithms/animation_transformations.h>

namespace nvl {

template<class A>
void animationLaplacianSmoothing(
        A& animation,
        const unsigned int iterations,
        const double alpha)
{
    animationFrameLaplacianSmoothing(animation.keyframes(), iterations, alpha);
}

template<class F>
void animationFrameLaplacianSmoothing(
        std::vector<F>& frames,
        const unsigned int iterations,
        const double alpha)
{
    typedef typename F::Transformation Transformation;

    if (frames.size() < 3)
        return;

    for (unsigned int it = 0; it < iterations; ++it) {
        for (Index i = 0; i < frames.size(); i++) {
            const double currentAlpha = alpha;

            if (!epsEqual(currentAlpha, 1.0)) {
                const double prevAlpha = (1.0 - alpha) / 2.0;
                const double nextAlpha = (1.0 - alpha) / 2.0;

                Index prevI = (i == 0 ? frames.size() - 1: i - 1);
                Index nextI = (i + 1) % frames.size();

                std::vector<Transformation>& transformations = frames[i].transformations();
                const std::vector<Transformation>& prevTransformations = frames[prevI].transformations();
                const std::vector<Transformation>& nextTransformations = frames[nextI].transformations();

                std::vector<double> weights(3);
                weights[0] = prevAlpha;
                weights[1] = currentAlpha;
                weights[2] = nextAlpha;

                for (Index jId = 0; jId < transformations.size(); ++jId) {
                    std::vector<Transformation> ts(3);
                    ts[0] = prevTransformations[jId];
                    ts[1] = transformations[jId];
                    ts[2] = nextTransformations[jId];

                    transformations[jId] = nvl::interpolateAffine(ts, weights);
                }
            }
        }
    }
}

template<class A>
void animationLaplacianSmoothing(
        A& animation,
        const unsigned int iterations,
        const std::vector<double>& alphas)
{
    animationFrameLaplacianSmoothing(animation.keyframes(), iterations, alphas);
}

template<class F>
void animationFrameLaplacianSmoothing(
        std::vector<F>& frames,
        const unsigned int iterations,
        const std::vector<double>& alphas)
{
    typedef typename F::Transformation Transformation;

    if (frames.size() < 3)
        return;

    for (unsigned int it = 0; it < iterations; ++it) {
        for (Index i = 0; i < frames.size(); i++) {
            const double currentAlpha = alphas[i];

            if (!epsEqual(currentAlpha, 1.0)) {
                const double prevAlpha = (1.0 - alphas[i]) / 2.0;
                const double nextAlpha = (1.0 - alphas[i]) / 2.0;

                Index prevI = (i == 0 ? frames.size() - 1 : i - 1);
                Index nextI = (i + 1) % frames.size();

                std::vector<Transformation>& transformations = frames[i].transformations();
                const std::vector<Transformation>& prevTransformations = frames[prevI].transformations();
                const std::vector<Transformation>& nextTransformations = frames[nextI].transformations();

                std::vector<double> weights(3);
                weights[0] = prevAlpha;
                weights[1] = currentAlpha;
                weights[2] = nextAlpha;

                for (Index jId = 0; jId < transformations.size(); ++jId) {
                    std::vector<Transformation> ts(3);
                    ts[0] = prevTransformations[jId];
                    ts[1] = transformations[jId];
                    ts[2] = nextTransformations[jId];

                    transformations[jId] = nvl::interpolateAffine(ts, weights);
                }
            }
        }
    }
}

template<class A>
void animationLaplacianSmoothing(
        A& animation,
        const unsigned int iterations,
        const std::vector<std::vector<double>>& alphas)
{
    animationFrameLaplacianSmoothing(animation.keyframes(), iterations, alphas);
}

template<class F>
void animationFrameLaplacianSmoothing(
        std::vector<F>& frames,
        const unsigned int iterations,
        const std::vector<std::vector<double>>& alphas)
{
    typedef typename F::Transformation Transformation;

    if (frames.size() < 3)
        return;

    for (unsigned int it = 0; it < iterations; ++it) {
        for (Index i = 0; i < frames.size(); i++) {

            Index prevI = (i == 0 ? frames.size() - 1 : i - 1);
            Index nextI = (i + 1) % frames.size();

            std::vector<Transformation>& transformations = frames[i].transformations();
            const std::vector<Transformation>& prevTransformations = frames[prevI].transformations();
            const std::vector<Transformation>& nextTransformations = frames[nextI].transformations();

            for (Index jId = 0; jId < transformations.size(); ++jId) {
                const double currentAlpha = alphas[i][jId];
                if (!epsEqual(currentAlpha, 1.0)) {
                    const double prevAlpha = (1.0 - alphas[i][jId]) / 2.0;
                    const double nextAlpha = (1.0 - alphas[i][jId]) / 2.0;

                    std::vector<double> weights(3);
                    weights[0] = prevAlpha;
                    weights[1] = currentAlpha;
                    weights[2] = nextAlpha;

                    std::vector<Transformation> ts(3);
                    ts[0] = prevTransformations[jId];
                    ts[1] = transformations[jId];
                    ts[2] = nextTransformations[jId];

                    transformations[jId] = nvl::interpolateAffine(ts, weights);
                }
            }
        }
    }
}

}
