/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MODEL_TRANSFER_H
#define NVL_MODELS_MODEL_TRANSFER_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Model>
void modelTransfer(
        const Model& model,
        const std::vector<typename Model::Mesh::FaceId>& faces,
        const std::vector<typename Model::Skeleton::JointId>& joints,
        Model& targetModel,
        std::vector<typename Model::Mesh::VertexId>& birthVertex,
        std::vector<typename Model::Mesh::FaceId>& birthFace,
        std::vector<typename Model::Skeleton::JointId>& birthJoint);

template<class Model>
void modelSkinningWeightsTransfer(
        const Model& model,
        const std::vector<typename Model::Mesh::VertexId>& birthVertex,
        const std::vector<typename Model::Skeleton::JointId>& birthJoint,
        Model& targetModel);

template<class Model>
void modelAnimationTransfer(
        const Model& model,
        const std::vector<typename Model::Skeleton::JointId>& birthJoint,
        Model& targetModel);

template<class Model>
void modelAnimationTransfer(
        const Model& model,
        typename Model::AnimationId animationId,
        const std::vector<typename Model::Skeleton::JointId>& birthJoint,
        Model& targetModel);

template<class Model>
void modelAnimationFrameTransfer(
        const Model& model,
        const typename Model::Animation::Frame& animationFrame,
        const std::vector<typename Model::Skeleton::JointId>& birthJoint,
        Model& targetModel,
        typename Model::Animation::Frame& newAnimationFrame);

}

#include "model_transfer.cpp"

#endif // NVL_MODELS_MODEL_TRANSFER_H
