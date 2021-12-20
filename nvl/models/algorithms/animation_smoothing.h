#ifndef NVL_MODELS_ANIMATION_SMOOTHING_H
#define NVL_MODELS_ANIMATION_SMOOTHING_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class A>
void animationLaplacianSmoothing(
        A& animation,
        const unsigned int iterations,
        const double alpha);

template<class F>
void animationFrameLaplacianSmoothing(
        std::vector<F>& frames,
        const unsigned int iterations,
        const double alpha);

template<class A>
void animationLaplacianSmoothing(
        A& animation,
        const unsigned int iterations,
        const std::vector<double>& alphas);

template<class F>
void animationFrameLaplacianSmoothing(
        std::vector<F>& frames,
        const unsigned int iterations,
        const std::vector<double>& alphas);

template<class A>
void animationLaplacianSmoothing(
        A& animation,
        const unsigned int iterations,
        const std::vector<std::vector<double>>& alphas);

template<class F>
void animationFrameLaplacianSmoothing(
        std::vector<F>& frames,
        const unsigned int iterations,
        const std::vector<std::vector<double>>& alphas);

}

#include "animation_smoothing.cpp"

#endif // NVL_MODELS_ANIMATION_SMOOTHING_H
