#ifndef NVL_MODELS_MODEL_TRANSFORMATIONS_H
#define NVL_MODELS_MODEL_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Model, class T>
void modelApplyTransformation(Model& model, const T& transformation, const bool removeNonStandardTransformations = true);

}

#include "model_transformations.cpp"

#endif // NVL_MODELS_MODEL_TRANSFORMATIONS_H
