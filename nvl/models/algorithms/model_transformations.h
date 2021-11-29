#ifndef NVL_MODELS_MODEL_TRANSFORMATIONS_H
#define NVL_MODELS_MODEL_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

#include <nvl/math/affine.h>

#include <vector>

namespace nvl {

template<class Model, class T>
void modelApplyTransformation(Model& model, const Affine3<T>& transformation);

template<class Model>
void modelRemoveRotationInBindPose(Model& model);

template<class S, class A>
void modelRemoveRotationInBindPose(S& skeleton, std::vector<A>& animations);

}

#include "model_transformations.cpp"

#endif // NVL_MODELS_MODEL_TRANSFORMATIONS_H
