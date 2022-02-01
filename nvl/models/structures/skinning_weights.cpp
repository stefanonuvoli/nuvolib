/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "skinning_weights.h"

namespace nvl {

template<class T>
SkinningWeights<T>::SkinningWeights()
{

}

template<class T>
void SkinningWeights<T>::clear()
{
    vSparseMatrix.resize(0,0);
}

template<class T>
void SkinningWeights<T>::initialize(const Size& vertexNumber, const Size& jointNumber)
{
    clear();
    vSparseMatrix.resize(static_cast<EigenId>(vertexNumber), static_cast<EigenId>(jointNumber));
}

template<class T>
void SkinningWeights<T>::resize(const Size& vertexNumber, const Size& jointNumber)
{
    clear();
    vSparseMatrix.conservativeResize(static_cast<EigenId>(vertexNumber), static_cast<EigenId>(jointNumber));
}

template<class T>
T SkinningWeights<T>::weight(const Index& vertexId, const Index& jointId) const
{
    return vSparseMatrix.coeff(vertexId, jointId);
}

template<class T>
T& SkinningWeights<T>::weight(const Index& vertexId, const Index& jointId)
{
    return vSparseMatrix.coeffRef(vertexId, jointId);
}

template<class T>
void SkinningWeights<T>::setWeight(const Index& vertexId, const Index& jointId, const T& weight)
{
    vSparseMatrix.coeffRef(vertexId, jointId) = weight;
}

template<class T>
const SparseMatrixd& SkinningWeights<T>::eigenMatrix() const
{
    return vSparseMatrix;
}

template<class T>
void SkinningWeights<T>::updateNonZeros()
{
    vSparseMatrix.prune(0.0);

    vNonZero.clear();
    vNonZero.resize(vSparseMatrix.innerSize(), std::vector<Index>());

    for (EigenId col = 0; col < vSparseMatrix.outerSize(); ++col) {
        for (SparseMatrix<double>::InnerIterator it(vSparseMatrix, col); it; ++it) {
            EigenId row = it.row();
            vNonZero[row].push_back(static_cast<Index>(col));
        }
    }
}

template<class T>
const std::vector<Index>& SkinningWeights<T>::nonZeroWeights(const Index& vertexId) const
{
    return vNonZero[vertexId];
}

template<class T>
const std::vector<std::vector<Index>>& SkinningWeights<T>::nonZeroWeights() const
{
    return vNonZero;
}

}
