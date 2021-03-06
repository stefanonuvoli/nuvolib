/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "animation_skinning.h"

namespace nvl {

/**
 * @brief Linear blending skinning
 * @param skinningWeights Skinning weights
 * @param transformations Transformation
 * @param vId Id of the vertex in the mesh
 * @return Resulting transformation for the vertex
 */
template<class W, class T>
T animationLinearBlendingSkinningVertex(
        const W& skinningWeights,
        const std::vector<T>& transformations,
        const Index& vId)
{
    typedef T Transformation;

    Matrix44d matrix;
    matrix.setZero();

    const std::vector<Index>& nonZeroValues = skinningWeights.nonZeroWeights(vId);

    if (nonZeroValues.empty())
        return Transformation::Identity();

    for (const Index& jointId : nonZeroValues) {
        const double& weight = skinningWeights.weight(vId, jointId);

        const Transformation& jointTransformation = transformations[jointId];

        matrix = matrix + (weight * jointTransformation.matrix());
    }

    return Transformation(matrix);
}

/**
 * @brief Dual quaternion skinning
 * @param skinningWeights Skinning weights
 * @param transformations Transformation
 * @param vId Id of the vertex in the mesh
 * @return Resulting transformation for the vertex
 */
template<class W, class T>
DualQuaternion<T> animationDualQuaternionSkinningVertex(
        const W& skinningWeights,
        const std::vector<DualQuaternion<T>>& transformations,
        const Index& vId)
{
    DualQuaternion<T> dq;
    dq.setZero();

    const std::vector<Index>& nonZeroValues = skinningWeights.nonZeroWeights(vId);

    if (nonZeroValues.empty())
        return DualQuaternion<T>::Identity();

    const Index& pivotJoint = nonZeroValues[0];

    for (const Index& jointId : nonZeroValues) {
        double weight = skinningWeights.weight(vId, jointId);

        const DualQuaternion<T>& jointTransformation = transformations[jointId];

        const double dot = transformations[pivotJoint].rotation().dot(transformations[jointId].rotation());
        if (dot < 0.0)
            weight *= -1; //We change sign if necessary

        dq = dq + (weight * jointTransformation);
    }

    dq.normalize();

    return dq;
}

}
