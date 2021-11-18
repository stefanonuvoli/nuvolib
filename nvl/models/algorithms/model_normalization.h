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
