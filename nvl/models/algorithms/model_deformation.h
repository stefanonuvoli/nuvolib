#ifndef NVL_MODELS_MODEL_DEFORMATION_H
#define NVL_MODELS_MODEL_DEFORMATION_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/models/algorithms/animation_skinning.h>

namespace nvl {

template<class M, class T>
void modelDeformLinearBlendingSkinning(
        M& model,
        const std::vector<T>& transformations);

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<DualQuaternion<T>>& transformations);

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<T>& transformations);

}

#include "model_deformation.cpp"

#endif // NVL_MODELS_MODEL_DEFORMATION_H
