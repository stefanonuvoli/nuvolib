#include "model_normalization.h"

#include <vector>

#include <nvl/math/comparisons.h>

namespace nvl {

template<class Model>
void modelNormalizeSkinningWeights(Model& model)
{
    typedef typename Model::Mesh Mesh;
    typedef typename Mesh::VertexId VertexId;

    const Mesh& mesh = model.mesh;

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (!mesh.isVertexDeleted(vId)) {
            modelNormalizeSkinningWeights(model, vId);
        }
    }
}

template<class Model>
void modelNormalizeSkinningWeights(Model& model, const typename Model::Mesh::VertexId& vId)
{
    typedef typename Model::Skeleton Skeleton;
    typedef typename Model::SkinningWeights SkinningWeights;
    typedef typename Skeleton::JointId JointId;
    typedef typename SkinningWeights::Scalar SkinningWeightsScalar;

    SkinningWeights& skinningWeights = model.skinningWeights;

    const std::vector<JointId>& nonZeroValues = skinningWeights.nonZeroWeights(vId);

    SkinningWeightsScalar sum = 0.0;
    for (const JointId& jId : nonZeroValues) {
        sum += skinningWeights.weight(vId, jId);
    }
    for (const JointId& jId : nonZeroValues) {
        SkinningWeightsScalar& weight = skinningWeights.weight(vId, jId);
        weight = weight / sum;
        assert(weight >= 0.0 && weight <= 1.0);
    }
}

}
