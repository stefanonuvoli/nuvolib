#ifndef NVL_MODELS_ANIMATION_ALGORITHMS_H
#define NVL_MODELS_ANIMATION_ALGORITHMS_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class S, class F>
void animationComputeFinalFromLocalFrames(
        const S& skeleton,
        std::vector<F>& frames);

template<class S, class F>
void animationGlobalFromLocalFrames(
        const S& skeleton,
        std::vector<F>& frames);
template<class S, class F>
void animationGlobalFromLocalFrame(
        const S& skeleton,
        F& frame);
template<class S, class F>
void animationGlobalFromLocalFrame(
        const S& skeleton,
        const typename S::JointId& currentId,
        F& frame);
template<class S, class T>
void animationGlobalFromLocalTransformations(
        const S& skeleton,
        std::vector<T>& transformations);
template<class S, class T>
void animationGlobalFromLocalTransformations(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations);

template<class S, class F>
void animationLocalFromGlobalFrames(
        const S& skeleton,
        std::vector<F>& frames);
template<class S, class F>
void animationLocalFromGlobalFrame(
        const S& skeleton,
        F& frame);
template<class S, class F>
void animationLocalFromGlobalFrame(
        const S& skeleton,
        const typename S::JointId& currentId,
        F& frame);
template<class S, class T>
void animationLocalFromGlobalTransformations(
        const S& skeleton,
        std::vector<T>& transformations);
template<class S, class T>
void animationLocalFromGlobalTransformations(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations);

}

#include "animation_algorithms.cpp"

#endif // NVL_MODELS_ANIMATION_ALGORITHMS_H
