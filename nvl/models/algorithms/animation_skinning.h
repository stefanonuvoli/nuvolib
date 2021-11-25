#ifndef NVL_MODELS_ANIMATION_SKINNING_H
#define NVL_MODELS_ANIMATION_SKINNING_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/math/dual_quaternion.h>
#include <nvl/math/matrix.h>

namespace nvl {

template<class W, class T>
T animationLinearBlendingSkinningVertex(
        const W& skinningWeights,
        const std::vector<T>& transformations,
        const Index& vId);

template<class W, class T>
DualQuaternion<T> animationDualQuaternionSkinningVertex(
        const W& skinningWeights,
        const std::vector<DualQuaternion<T>>& transformations,
        const Index& vId);

}

#include "animation_skinning.cpp"

#endif // NVL_MODELS_ANIMATION_SKINNING_H
