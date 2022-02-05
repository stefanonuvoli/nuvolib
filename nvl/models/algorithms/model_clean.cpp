/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "model_clean.h"

#include <nvl/math/transformations.h>

#include <nvl/models/algorithms/animation_clean.h>
#include <nvl/models/algorithms/skeleton_clean.h>
#include <nvl/models/algorithms/skeleton_bind_pose.h>

namespace nvl {

/**
 * @brief Remove non standard-transformations from model (scaling and rotation
 * from bind pose transformations and scaling from animations)
 * @param model Model
 */
template<class Model>
void modelRemoveNonStandardTransformations(Model& model)
{
    return modelRemoveNonStandardTransformations(model.skeleton, model.animations);
}

/**
 * @brief Remove non standard-transformations from model (scaling and rotation
 * from bind pose transformations and scaling from animations)
 * @param model Model
 */
template<class S, class A>
void modelRemoveNonStandardTransformations(S& skeleton, std::vector<A>& animations)
{
    typedef S Skeleton;
    typedef A Animation;
    typedef typename Animation::FrameId FrameId;
    typedef typename Animation::Transformation AnimationTransformation;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Transformation SkeletonTransformation;
    typedef typename Skeleton::Scalar SkeletonScalar;
    typedef typename SkeletonTransformation::LinearMatrixType SkeletonLinearMatrixType;
    typedef typename AnimationTransformation::LinearMatrixType AnimationLinearMatrixType;
    typedef typename Animation::Scalar AnimationScalar;

    //Remove scaling from bind poses and animations
    modelRemoveScaling(skeleton, animations);

    //Compute local bind pose removing the local bind pose
    std::vector<SkeletonTransformation> oldLocalInverseBindPose = skeletonLocalInverseBindPose(skeleton);

    #pragma omp parallel for
    for (Index aId = 0; aId < animations.size(); ++aId) {
        Animation& animation = animations[aId];

        animationLocalFromGlobal(skeleton, animation);
        animationApplyTransformation(animation, oldLocalInverseBindPose);
    }

    //For each joint
    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        SkeletonTransformation bindPose = skeleton.jointBindPose(jId);

        //Get data
        SkeletonLinearMatrixType rotMatrix, scalMatrix;
        bindPose.computeRotationScaling(&rotMatrix, &scalMatrix);
        Vector3<SkeletonScalar> bindTraVec(bindPose.translation());
        Rotation3<SkeletonScalar> bindRot(rotMatrix);
        Vector3<SkeletonScalar> bindScaVec(scalMatrix.diagonal());

        if (bindRot.isApprox(Rotation3<SkeletonScalar>::Identity()) && bindScaVec.isApprox(Vector3<SkeletonScalar>::Ones()))
            continue;

        //Get the resulting vector
        Vector3<SkeletonScalar> vec = bindPose * Vector3<SkeletonScalar>::Zero();
        Translation3<SkeletonScalar> tra(vec);

        //New bind pose
        SkeletonTransformation newBindPose = tra * SkeletonTransformation::Identity();

        //Adjust rotation for each animation
        for (Index aId = 0; aId < animations.size(); ++aId) {
            Animation& animation = animations[aId];

            for (FrameId fId = 0; fId < animation.keyframeNumber(); ++fId) {
                AnimationTransformation& t = animation.keyframe(fId).transformation(jId);

                //Get data
                AnimationLinearMatrixType rotMatrix, scalMatrix;
                t.computeRotationScaling(&rotMatrix, &scalMatrix);
                Vector3<AnimationScalar> animTraVec(t.translation());
                Rotation3<AnimationScalar> animRot(rotMatrix);
                Vector3<AnimationScalar> animScaVec(scalMatrix.diagonal());

                //Get rotation transformation informations
                AnimationScalar angle = animRot.angle();
                Vector3<AnimationScalar> axis = animRot.axis();
                axis = bindRot * axis;

                animTraVec = bindRot * animTraVec;

                //Set new transformation
                t.fromPositionOrientationScale(
                    animTraVec,
                    Rotation3<AnimationScalar>(angle, axis),
                    animScaVec);
            }
        }

        skeleton.setJointBindPose(jId, newBindPose);
    }

    //Compute new local bind pose
    std::vector<SkeletonTransformation> newLocalBindPose = skeletonLocalBindPose(skeleton);

    #pragma omp parallel for
    for (Index aId = 0; aId < animations.size(); ++aId) {
        Animation& animation = animations[aId];

        animationApplyTransformation(animation, newLocalBindPose);
        animationGlobalFromLocal(skeleton, animation);
    }
}

/**
 * @brief Remove scaling from model transformations
 * @param model Model
 */
template<class Model>
void modelRemoveScaling(Model& model)
{
    return modelRemoveScaling(model.skeleton, model.animations);
}

/**
 * @brief Remove scaling from model transformations
 * @param model Model
 */
template<class S, class A>
void modelRemoveScaling(S& skeleton, std::vector<A>& animations)
{
    skeletonRemoveScaling(skeleton);
    animationRemoveScaling(skeleton, animations);
}

}
