#ifndef NVL_MODELS_SKINNING_WEIGHTS_H
#define NVL_MODELS_SKINNING_WEIGHTS_H

#include <nvl/nuvolib.h>

#include <nvl/math/sparsematrix.h>

#include <vector>

namespace nvl {

template<class T = double>
class SkinningWeights
{

public:

    typedef T Scalar;

    SkinningWeights();

    void clear();
    void initialize(const Size& vertexNumber, const Size& jointNumber);
    void resize(const Size& vertexNumber, const Size& jointNumber);

    T weight(const Index& vertexId, const Index& jointId) const;
    T& weight(const Index& vertexId, const Index& jointId);
    void setWeight(const Index& vertexId, const Index& jointId, const T& weight);

    const SparseMatrixd& eigenMatrix() const;

    void updateNonZeros();
    const std::vector<Index>& nonZeroWeights(const Index& vertexId) const;
    const std::vector<std::vector<Index>>& nonZeroWeights() const;

protected:

    std::vector<std::vector<Index>> vNonZero;

    SparseMatrixd vSparseMatrix;

};

}

#include "skinning_weights.cpp"

#endif // NVL_MODELS_SKINNING_WEIGHTS_H
