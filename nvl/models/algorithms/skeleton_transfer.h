/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_SKELETON_TRANSFER_H
#define NVL_MODELS_SKELETON_TRANSFER_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Skeleton>
void skeletonTransferJoints(
        const Skeleton& skeleton,
        const std::vector<typename Skeleton::JointId>& joints,
        Skeleton& targetSkeleton,
        std::vector<typename Skeleton::JointId>& birthJoint,
        typename Skeleton::JointId targetParentId = NULL_ID);

}

#include "skeleton_transfer.cpp"

#endif // NVL_MODELS_SKELETON_TRANSFER_H
