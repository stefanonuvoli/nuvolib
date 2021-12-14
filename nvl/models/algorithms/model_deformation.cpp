#include "model_deformation.h"

#include <nvl/models/algorithms/animation_poses.h>
#include <nvl/models/algorithms/model_transformations.h>
#include <nvl/models/algorithms/animation_transformations.h>
#include <nvl/models/algorithms/model_clean.h>

namespace nvl {

template<class M, class S, class W, class A, class T>
void modelDeformLinearBlendingSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<T>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations)
{
    typedef typename M::Point Point;
    typedef typename M::VertexNormal VertexNormal;

    //Apply transformations to skeleton
    skeletonApplyTransformation(skeleton, transformations);

    //Skinning on vertices
    #pragma omp parallel for
    for (Index vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (!mesh.isVertexDeleted(vId)) {
            T t = animationLinearBlendingSkinningVertex(skinningWeights, transformations, vId);

            const Point& p = mesh.vertexPoint(vId);
            mesh.setVertexPoint(vId, t * p);

            if (mesh.hasVertexNormals()) {
                const VertexNormal& n = mesh.vertexNormal(vId);
                mesh.setVertexNormal(vId, t.rotation() * n);
            }
        }
    }

    //Preserve animations
    if (!preserveAnimations) {
        animationApplyTransformation(animations, transformations);
    }

    //Remove scaling and rotation in bind pose
    if (!removeNonStandardTransformations) {
        modelRemoveNonStandardTransformations(skeleton, animations);
    }
}

template<class M, class T>
void modelDeformLinearBlendingSkinning(
        M& model,
        const std::vector<T>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations)
{
    return modelDeformLinearBlendingSkinning(model.mesh, model.skeleton, model.skinningWeights, model.animations, transformations, preserveAnimations, removeNonStandardTransformations);
}

template<class M, class S, class W, class A, class T>
void modelDeformDualQuaternionSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<DualQuaternion<T>>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations)
{
    typedef typename M::Point Point;
    typedef typename M::VertexNormal VertexNormal;
    typedef typename S::JointId JointId;
    typedef typename S::Transformation SkeletonTransformation;

    typedef S Skeleton;
    typedef A Animation;
    typedef typename Animation::FrameId FrameId;
    typedef typename Animation::Transformation AnimationTransformation;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Transformation SkeletonTransformation;
    typedef typename AnimationTransformation::LinearMatrixType AnimationLinearMatrixType;
    typedef typename Animation::Scalar AnimationScalar;

    //Compute local bind pose removing the local bind pose
    std::vector<SkeletonTransformation> oldLocalInverseBindPose;
    if (!preserveAnimations) {
        oldLocalInverseBindPose = skeletonLocalInverseBindPose(skeleton);
    }

    //Apply transformations to skeleton
    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        const DualQuaternion<T>& dq = transformations[jId];

        SkeletonTransformation r = dq.affineTransformation() * skeleton.jointBindPose(jId);
        skeleton.setJointBindPose(jId, r);
    }

    //Skinning on vertices
    #pragma omp parallel for
    for (Index vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (!mesh.isVertexDeleted(vId)) {
            DualQuaternion<T> dq = animationDualQuaternionSkinningVertex(skinningWeights, transformations, vId);

            const Point& p = mesh.vertexPoint(vId);
            mesh.setVertexPoint(vId, dq * p);

            if (mesh.hasVertexNormals()) {
                const VertexNormal& n = mesh.vertexNormal(vId);
                mesh.setVertexNormal(vId, dq.rotation() * n);
            }
        }
    }

    //Preserve animations
    if (!preserveAnimations) {
        //Compute new local bind pose
        std::vector<SkeletonTransformation> newLocalBindPose = skeletonLocalBindPose(skeleton);

        #pragma omp parallel for
        for (Index aId = 0; aId < animations.size(); ++aId) {
            Animation& animation = animations[aId];

            animationLocalFromGlobal(skeleton, animation);
            animationApplyTransformation(animation, oldLocalInverseBindPose);
        }

        //Adjust rotation for each animation
        #pragma omp parallel for
        for (Index aId = 0; aId < animations.size(); ++aId) {
            Animation& animation = animations[aId];

            for (FrameId fId = 0; fId < animation.keyframeNumber(); ++fId) {
                for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
                    AnimationTransformation& t = animation.keyframe(fId).transformation(jId);
                    const DualQuaternion<T>& dq = transformations[jId];

                    Quaternion<T> dqRot(dq.rotation());
                    if (dqRot.isApprox(Quaternion<AnimationScalar>::Identity()))
                        continue;

                    //Get data
                    AnimationLinearMatrixType rotMatrix, scalMatrix;
                    t.computeRotationScaling(&rotMatrix, &scalMatrix);
                    Vector3<AnimationScalar> animTraVec(t.translation());
                    Rotation3<AnimationScalar> animRot(rotMatrix);
                    Vector3<AnimationScalar> animScaVec(scalMatrix.diagonal());

                    //Get rotation transformation informations
                    AnimationScalar angle = animRot.angle();
                    Vector3<AnimationScalar> axis = animRot.axis();
                    axis = dqRot * axis;

                    animTraVec = dqRot * animTraVec;

                    //Set new transformation
                    t.fromPositionOrientationScale(
                        animTraVec,
                        Rotation3<AnimationScalar>(angle, axis),
                        animScaVec);
                }
            }
        }

        #pragma omp parallel for
        for (Index aId = 0; aId < animations.size(); ++aId) {
            Animation& animation = animations[aId];

            animationApplyTransformation(animation, newLocalBindPose);
            animationGlobalFromLocal(skeleton, animation);
        }
    }

    //Remove scaling and rotation in bind pose
    if (!removeNonStandardTransformations) {
        modelRemoveNonStandardTransformations(skeleton, animations);
    }
}

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<DualQuaternion<T>>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations)
{
    return modelDeformDualQuaternionSkinning(model.mesh, model.skeleton, model.skinningWeights, model.animations, transformations, preserveAnimations, removeNonStandardTransformations);
}

template<class M, class S, class W, class A, class T>
void modelDeformDualQuaternionSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<T>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations)
{

    typedef typename T::Scalar Scalar;

    std::vector<DualQuaternion<Scalar>> dualQuaternionTransformations(transformations.size());

    #pragma omp parallel for
    for (Index i = 0; i < transformations.size(); ++i) {
        dualQuaternionTransformations[i] = DualQuaternion<Scalar>(Quaternion<Scalar>(transformations[i].rotation()), transformations[i].translation());
    }

    modelDeformDualQuaternionSkinning(mesh, skeleton, skinningWeights, animations, dualQuaternionTransformations, preserveAnimations, removeNonStandardTransformations);
}

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<T>& transformations,
        const bool preserveAnimations,
        const bool removeNonStandardTransformations)
{
    return modelDeformDualQuaternionSkinning(model.mesh, model.skeleton, model.skinningWeights, model.animations, transformations, preserveAnimations, removeNonStandardTransformations);
}

}
