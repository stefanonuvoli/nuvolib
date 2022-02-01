/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
