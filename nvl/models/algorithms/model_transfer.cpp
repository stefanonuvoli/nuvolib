/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "model_transfer.h"

#include <nvl/models/algorithms/mesh_transfer.h>
#include <nvl/models/algorithms/skeleton_transfer.h>

namespace nvl {

/**
 * @brief Transfer data from a model to another
 * @param model Model
 * @param faces Faces to transfer
 * @param joints Joints to transfer
 * @param targetModel Target model
 * @param birthVertex Birth vertex of the resulting model
 * @param birthFace Birth vertex of the resulting model
 * @param birthJoint Birth joint of the resulting model
 */
template<class Model>
void modelTransfer(
        const Model& model,
        const std::vector<typename Model::Mesh::FaceId>& faces,
        const std::vector<typename Model::Skeleton::JointId>& joints,
        Model& targetModel,
        std::vector<typename Model::Mesh::VertexId>& birthVertex,
        std::vector<typename Model::Mesh::FaceId>& birthFace,
        std::vector<typename Model::Skeleton::JointId>& birthJoint)
{
    meshTransferFaces(model.mesh, faces, targetModel.mesh, birthVertex, birthFace);
    skeletonTransferJoints(model.skeleton, joints, targetModel.skeleton, birthJoint);
    modelSkinningWeightsTransfer(model, birthVertex, birthJoint, targetModel);
    modelAnimationTransfer(model, birthJoint, targetModel);
}

/**
 * @brief Transfer skinning weights from a model to another
 * @param model Model
 * @param birthVertex Birth vertex of the resulting model
 * @param birthJoint Birth joint of the resulting model
 * @param targetModel Target model
 */
template<class Model>
void modelSkinningWeightsTransfer(
        const Model& model,
        const std::vector<typename Model::Mesh::VertexId>& birthVertex,
        const std::vector<typename Model::Skeleton::JointId>& birthJoint,
        Model& targetModel)
{
    typedef typename Model::Mesh::VertexId VertexId;
    typedef typename Model::Skeleton::JointId JointId;

    targetModel.initializeSkinningWeights();

    for (VertexId vId = 0; vId < targetModel.mesh.nextVertexId(); ++vId) {
        if (targetModel.mesh.isVertexDeleted(vId) || birthVertex[vId] == NULL_ID)
            continue;

        for (JointId jId = 0; jId < targetModel.skeleton.jointNumber(); ++jId) {
            if (birthJoint[jId] == NULL_ID)
                continue;

            targetModel.skinningWeights.setWeight(vId, jId, model.skinningWeights.weight(birthVertex[vId], birthJoint[jId]));
        }
    }

    targetModel.skinningWeights.updateNonZeros();
}

/**
 * @brief Transfer skinning weights of all vertices from a model to another
 * @param model Model
 * @param birthVertex Birth vertex of the resulting model
 * @param targetModel Target model
 */
template<class Model>
void modelSkinningWeightsTransferVertices(
        const Model& model,
        const std::vector<typename Model::Mesh::VertexId>& birthVertex,
        Model& targetModel)
{
    typedef typename Model::Mesh::FaceId FaceId;
    typedef typename Model::Skeleton::JointId JointId;

    targetModel.initializeSkinningWeights();

    for (FaceId vId = 0; vId < targetModel.mesh.nextVertexId(); ++vId) {
        if (targetModel.mesh.isVertexDeleted(vId) || birthVertex[vId] == NULL_ID)
            continue;

        for (JointId jId = 0; jId < targetModel.skeleton.jointNumber(); ++jId) {
            targetModel.skinningWeights.setWeight(vId, jId, model.skinningWeights.weight(birthVertex[vId], jId));
        }
    }

    targetModel.skinningWeights.updateNonZeros();
}

/**
 * @brief Transfer skinning weights of all skeleton joints from a model to another
 * @param model Model
 * @param birthJoint Birth joint of the resulting model
 * @param targetModel Target model
 */
template<class Model>
void modelSkinningWeightsTransferJoints(
        const Model& model,
        const std::vector<Index>& birthJoint,
        Model& targetModel)
{
    typedef typename Model::Mesh::FaceId FaceId;
    typedef typename Model::Skeleton::JointId JointId;

    targetModel.initializeSkinningWeights();

    for (FaceId vId = 0; vId < targetModel.mesh.nextVertexId(); ++vId) {
        if (targetModel.mesh.isVertexDeleted(vId))
            continue;

        for (JointId jId = 0; jId < targetModel.skeleton.jointNumber(); ++jId) {
            if (birthJoint[jId] == NULL_ID)
                continue;

            targetModel.skinningWeights.setWeight(vId, birthJoint[jId], model.skinningWeights.weight(vId, birthJoint(jId)));
        }
    }

    targetModel.skinningWeights.updateNonZeros();
}

/**
 * @brief Transfer animations from a model to another
 * @param model Model
 * @param birthJoint Birth joint of the resulting model
 * @param targetModel Target model
 */
template<class Model>
void modelAnimationTransfer(
        const Model& model,
        const std::vector<typename Model::Skeleton::JointId>& birthJoint,
        Model& targetModel)
{
    typedef typename Model::AnimationId AnimationId;

    for (AnimationId aId = 0; aId < model.animationNumber(); ++aId) {
        modelAnimationTransfer(model, aId, birthJoint, targetModel);
    }
}

/**
 * @brief Transfer animation from a model to another
 * @param model Model
 * @param animationId Input animation
 * @param birthJoint Birth joint of the resulting model
 * @param targetModel Target model
 */
template<class Model>
void modelAnimationTransfer(
        const Model& model,
        typename Model::AnimationId animationId,
        const std::vector<typename Model::Skeleton::JointId>& birthJoint,
        Model& targetModel)
{
    typedef typename Model::Animation Animation;
    typedef typename Model::Animation::Frame AnimationFrame;

    const Animation& animation = model.animation(animationId);
    Animation newAnimation;

    newAnimation.setName(animation.name());

    for (const AnimationFrame& frame : animation.keyframes()) {
        AnimationFrame newFrame;
        modelAnimationFrameTransfer(model, frame, birthJoint, targetModel, newFrame);
        newAnimation.addKeyframe(newFrame);
    }

    targetModel.addAnimation(newAnimation);
}

/**
 * @brief Transfer animation frame from a model to another
 * @param model Model
 * @param animationFrame Input animation frame
 * @param birthJoint Birth joint of the resulting model
 * @param targetModel Target model
 * @param newAnimationFrame Resulting animation frame
 */
template<class Model>
void modelAnimationFrameTransfer(
        const Model& model,
        const typename Model::Animation::Frame& animationFrame,
        const std::vector<typename Model::Skeleton::JointId>& birthJoint,
        Model& targetModel,
        typename Model::Animation::Frame& newAnimationFrame)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(model);

    typedef typename Model::Animation::Transformation Transformation;
    typedef typename Model::Skeleton::JointId JointId;

    newAnimationFrame.setTime(animationFrame.time());

    const std::vector<Transformation>& transformations = animationFrame.transformations();
    std::vector<Transformation>& newTransformations = newAnimationFrame.transformations();

    newTransformations.resize(targetModel.skeleton.jointNumber());

    for (JointId jId = 0; jId < targetModel.skeleton.jointNumber(); ++jId) {
        if (birthJoint[jId] == NULL_ID)
            continue;

        newTransformations[jId] = transformations[birthJoint[jId]];
    }
}

}
