/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "skeleton_poses.h"

#include <nvl/models/algorithms/skeleton_poses.h>

namespace nvl {

/* ----------------------- GLOBAL ----------------------- */

template<class S>
std::vector<typename S::Transformation> skeletonGlobalBindPose(
        const S& skeleton)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Transformation Transformation;

    std::vector<Transformation> bindPose(skeleton.jointNumber());

    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        bindPose[jId] = skeleton.jointBindPose(jId);
    }

    return bindPose;
}

template<class S>
std::vector<typename S::Transformation> skeletonGlobalInverseBindPose(
        const S& skeleton)
{
    typedef S Skeleton;
    typedef typename Skeleton::Transformation Transformation;

    std::vector<Transformation> inverseBindPose = skeletonGlobalBindPose(skeleton);

    #pragma omp parallel for
    for (Index jId = 0; jId < inverseBindPose.size(); ++jId) {
        inverseBindPose[jId] = inverseBindPose[jId].inverse();
    }

    return inverseBindPose;
}

template<class S>
void skeletonSetGlobalBindPose(
        S& skeleton,
        const std::vector<typename S::Transformation>& bindPose)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        skeleton.setJointBindPose(jId, bindPose[jId]);
    }
}

/* ----------------------- LOCAL ----------------------- */

template<class S>
std::vector<typename S::Transformation> skeletonLocalBindPose(
        const S& skeleton)
{
    typedef S Skeleton;
    typedef typename Skeleton::Transformation Transformation;

    std::vector<Transformation> localBindPose = skeletonGlobalBindPose(skeleton);
    skeletonPoseLocalFromGlobal(skeleton, localBindPose);

    return localBindPose;
}

template<class S>
std::vector<typename S::Transformation> skeletonLocalInverseBindPose(
        const S& skeleton)
{
    typedef S Skeleton;
    typedef typename Skeleton::Transformation Transformation;

    std::vector<Transformation> inverseBindPose = skeletonLocalBindPose(skeleton);

    #pragma omp parallel for
    for (Index jId = 0; jId < inverseBindPose.size(); ++jId) {
        inverseBindPose[jId] = inverseBindPose[jId].inverse();
    }

    return inverseBindPose;
}

template<class S>
void skeletonSetLocalBindPose(
        S& skeleton,
        const std::vector<typename S::Transformation>& bindPose)
{
    typedef S Skeleton;
    typedef typename Skeleton::Transformation Transformation;

    std::vector<Transformation> globalBindPose = bindPose;
    skeletonPoseGlobalFromLocal(skeleton, globalBindPose);

    skeletonSetGlobalBindPose(skeleton, globalBindPose);
}


}
