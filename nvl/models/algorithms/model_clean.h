/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MODEL_CLEAN_H
#define NVL_MODELS_MODEL_CLEAN_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Model>
void modelRemoveNonStandardTransformations(Model& model);

template<class S, class A>
void modelRemoveNonStandardTransformations(S& skeleton, std::vector<A>& animations);

template<class Model>
void modelRemoveScaling(Model& model);

template<class S, class A>
void modelRemoveScaling(S& skeleton, std::vector<A>& animations);

}

#include "model_clean.cpp"

#endif // NVL_MODELS_MODEL_CLEAN_H
