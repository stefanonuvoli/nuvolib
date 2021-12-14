#ifndef NVL_MODELS_MODEL_DEFORMATION_H
#define NVL_MODELS_MODEL_DEFORMATION_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/models/algorithms/animation_skinning.h>

namespace nvl {

template<class M, class S, class W, class A, class T>
void modelDeformLinearBlendingSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<T>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations);
template<class M, class T>
void modelDeformLinearBlendingSkinning(
        M& model,
        const std::vector<T>& transformationss,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations);

template<class M, class S, class W, class A, class T>
void modelDeformDualQuaternionSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<DualQuaternion<T>>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations);
template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<DualQuaternion<T>>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations);

template<class M, class S, class W, class A, class T>
void modelDeformDualQuaternionSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<T>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations);
template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<T>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations);

}

#include "model_deformation.cpp"

#endif // NVL_MODELS_MODEL_DEFORMATION_H
