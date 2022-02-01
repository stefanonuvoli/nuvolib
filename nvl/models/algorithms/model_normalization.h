/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MODEL_NORMALIZATION_H
#define NVL_MODELS_MODEL_NORMALIZATION_H

#include <nvl/nuvolib.h>

namespace nvl {

template<class Model>
void modelNormalizeSkinningWeights(Model& model);

template<class Model>
void modelNormalizeSkinningWeights(Model& model, const typename Model::Mesh::VertexId& vId);

}

#include "model_normalization.cpp"

#endif // NVL_MODELS_MODEL_NORMALIZATION_H
