#ifndef NVL_MODELS_ANIMATION_SKINNING_H
#define NVL_MODELS_ANIMATION_SKINNING_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/math/dual_quaternion.h>
#include <nvl/math/matrix.h>

namespace nvl {

template<class SW, class T>
T animationLinearBlendingSkinningVertex(
        const SW& skinningWeights,
        const std::vector<T>& transformations,
        const Index& vId);

template<class SW, class T>
DualQuaternion<T> animationDualQuaternionSkinningVertex(
        const SW& skinningWeights,
        const std::vector<DualQuaternion<T>>& transformations,
        const Index& vId);

}

#include "animation_skinning.cpp"

#endif // NVL_MODELS_ANIMATION_SKINNING_H
