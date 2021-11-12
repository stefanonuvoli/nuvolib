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
        typename Skeleton::JointId targetParentId = MAX_INDEX);

}

#include "skeleton_transfer.cpp"

#endif // NVL_MODELS_SKELETON_TRANSFER_H
