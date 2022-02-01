/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "skeleton_clean.h"

#include <nvl/math/transformations.h>

#include <nvl/models/algorithms/skeleton_poses.h>
#include <nvl/models/algorithms/skeleton_bind_pose.h>

namespace nvl {

template<class S>
void skeletonRemoveScaling(S& skeleton)
{
    typedef S Skeleton;
    typedef typename Skeleton::Scalar Scalar;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Transformation Transformation;
    typedef typename Transformation::LinearMatrixType LinearMatrixType;

    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        Transformation& t = skeleton.jointBindPose(jId);

        //Get data
        LinearMatrixType rotMatrix, scalMatrix;
        t.computeRotationScaling(&rotMatrix, &scalMatrix);
        Vector3<Scalar> poseScaVec(scalMatrix.diagonal());

        if (poseScaVec.isApprox(Vector3<Scalar>::Ones()))
            continue;

        Vector3<Scalar> poseTraVec(t.translation());
        Rotation3<Scalar> poseRot(rotMatrix);

        t.fromPositionOrientationScale(
            poseTraVec,
            poseRot,
            Vector3<Scalar>::Ones());
    }
}

}
