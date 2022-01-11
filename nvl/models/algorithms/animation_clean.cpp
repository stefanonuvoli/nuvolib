#include "animation_clean.h"

#include <nvl/math/transformations.h>
#include <nvl/models/algorithms/animation_clean.h>
#include <nvl/models/algorithms/animation_poses.h>

#include <nvl/math/rotation.h>

namespace nvl {

template<class S, class A>
void animationRemoveScaling(const S& skeleton, std::vector<A>& animations)
{
    #pragma omp parallel for
    for (Index aId = 0; aId < animations.size(); ++aId) {
        animationRemoveScaling(skeleton, animations[aId]);
    }
}

template<class S, class A>
void animationRemoveScaling(const S& skeleton, A& animation)
{
    animationFrameRemoveScaling(skeleton, animation.keyframes());
}

template<class S, class F>
void animationFrameRemoveScaling(const S& skeleton, std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameRemoveScaling(skeleton, frames[fId]);
    }
}

template<class S, class F>
void animationFrameRemoveScaling(const S& skeleton, F& frame)
{
    typedef S Skeleton;
    typedef F Frame;
    typedef typename Skeleton::JointId JointId;
    typedef typename Frame::Transformation Transformation;
    typedef typename Frame::Scalar Scalar;
    typedef typename Transformation::LinearMatrixType LinearMatrixType;

    //For each joint
    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        Transformation& t = frame.transformation(jId);

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

template<class S, class A>
void animationRemoveRootMotion(const S& skeleton, std::vector<A>& animations)
{
    #pragma omp parallel for
    for (Index aId = 0; aId < animations.size(); ++aId) {
        animationRemoveRootMotion(skeleton, animations[aId]);
    }
}

template<class S, class A>
void animationRemoveRootMotion(const S& skeleton, A& animation)
{
    animationFrameRemoveRootMotion(skeleton, animation.keyframes());
}

template<class S, class F>
void animationFrameRemoveRootMotion(const S& skeleton, std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameRemoveRootMotion(skeleton, frames[fId]);
    }
}

template<class S, class F>
void animationFrameRemoveRootMotion(const S& skeleton, F& frame)
{
    typedef S Skeleton;
    typedef F Frame;
    typedef typename Skeleton::JointId JointId;
    typedef typename Frame::Transformation Transformation;
    typedef typename Frame::Scalar Scalar;
    typedef typename Transformation::LinearMatrixType LinearMatrixType;

    animationFrameLocalFromGlobal(skeleton, frame);

    //For each joint
    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        if (skeleton.isRoot(jId)) {
            Transformation& t = frame.transformation(jId);

            Vector3<Scalar> poseTraVec(t.translation());

            if (poseTraVec.isApprox(Vector3<Scalar>::Zero()))
                continue;

            //Get data
            LinearMatrixType rotMatrix, scalMatrix;
            t.computeRotationScaling(&rotMatrix, &scalMatrix);
            Vector3<Scalar> poseScaVec(scalMatrix.diagonal());

            Rotation3<Scalar> poseRot(rotMatrix);

            t.fromPositionOrientationScale(
                        Vector3<Scalar>::Zero(),
                        poseRot,
                        poseScaVec);
        }
    }

    animationFrameGlobalFromLocal(skeleton, frame);
}


}
