#ifndef NVL_MODELS_MODEL_TRANSFORMATIONS_H
#define NVL_MODELS_MODEL_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

#include <nvl/math/affine.h>

namespace nvl {

template<class Model, class T>
void modelApplyTransformation(Model& model, const Affine3<T>& transformation);

template<class Model>
void modelRemoveRotationInRestPose(Model& model);

}

#include "model_transformations.cpp"

#endif // NVL_MODELS_MODEL_TRANSFORMATIONS_H
