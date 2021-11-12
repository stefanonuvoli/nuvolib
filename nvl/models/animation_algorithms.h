#ifndef NVL_MODELS_ANIMATION_ALGORITHMS_H
#define NVL_MODELS_ANIMATION_ALGORITHMS_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class S, class F>
std::vector<F> animationComputeFinalTransformations(
        const S& skeleton,
        const std::vector<F>& frames);

template<class S, class F>
void animationComputeGlobalFrames(
        const S& skeleton,
        std::vector<F>& frames);
template<class S, class F>
void animationComputeGlobalFrame(
        const S& skeleton,
        F& frame);
template<class S, class F>
void animationComputeGlobalFrame(
        const S& skeleton,
        const typename S::JointId& currentId,
        F& frame);
template<class S, class T>
void animationComputeGlobalTransformations(
        const S& skeleton,
        std::vector<T>& transformations);
template<class S, class T>
void animationComputeGlobalTransformations(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations);

template<class S, class F>
void animationComputeLocalFrames(
        const S& skeleton,
        std::vector<F>& frames);
template<class S, class F>
void animationComputeLocalFrame(
        const S& skeleton,
        F& frame);
template<class S, class F>
void animationComputeLocalFrame(
        const S& skeleton,
        const typename S::JointId& currentId,
        F& frame);
template<class S, class T>
void animationComputeLocalTransformations(
        const S& skeleton,
        std::vector<T>& transformations);
template<class S, class T>
void animationComputeLocalTransformations(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations);

template<class F>
void animationBlendFrameTransformations(
        std::vector<F>& animationFrames,
        const double& fps = 60,
        const double& speed = 1,
        const bool& keepKeyframes = true);

}

#include "animation_algorithms.cpp"

#endif // NVL_MODELS_ANIMATION_ALGORITHMS_H
