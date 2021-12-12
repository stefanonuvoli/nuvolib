#include "model_deformation.h"

#include <nvl/models/algorithms/animation_poses.h>
#include <nvl/models/algorithms/model_transformations.h>

namespace nvl {

template<class M, class S, class W, class A, class T>
void modelDeformLinearBlendingSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<T>& transformations,
        const bool keepRotationInBindPose,
        const bool preserveAnimations)
{
    typedef typename S::JointId JointId;
    typedef typename S::Scalar SkeletonScalar;
    typedef typename S::Transformation SkeletonTransformation;

    typedef typename A::Transformation AnimationTransformation;
    typedef typename A::Frame Frame;
    typedef typename A::FrameId FrameId;

    std::vector<A> globalAnimations;

    if (preserveAnimations) {
        std::vector<SkeletonTransformation> localBindPose = skeletonLocalBindPose(skeleton);

        globalAnimations.resize(animations.size());
        for (Index i = 0; i < animations.size(); ++i) {
            globalAnimations[i] = animations[i];

            #pragma omp parallel for
            for (nvl::Index fId = 0; fId < globalAnimations[i].keyframeNumber(); ++fId) {
                for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
                    AnimationTransformation& t = globalAnimations[i].keyframe(fId).transformation(jId);

                    t = localBindPose[jId] * t;
                }
            }

            animationGlobalFromLocal(skeleton, globalAnimations[i]);
        }
    }

    #pragma omp parallel for
    for (Index jId = 0; jId < skeleton.jointNumber(); ++jId) {
        const T& t = transformations[jId];

        SkeletonTransformation r;

        if (keepRotationInBindPose || preserveAnimations) {
            r = t * skeleton.jointBindPose(jId);
        }
        else {
            Point3d p = skeleton.jointBindPose(jId) * Point3<SkeletonScalar>::Zero();
            p = t * p;

            Translation3d jointTranslation(p);

            r = jointTranslation * SkeletonTransformation::Identity();

            Matrix33<SkeletonScalar> rotMatrix, scalMatrix;
            t.computeRotationScaling(&rotMatrix, &scalMatrix);

            const Quaternion<SkeletonScalar> tRotation(rotMatrix);
            const Scaling3<SkeletonScalar> tScaling(scalMatrix.diagonal());

            for (A& animation : animations) {
                std::vector<Frame>& frames = animation.keyframes();
                for (FrameId fId = 0; fId < frames.size(); ++fId) {
                    AnimationTransformation& animationTransformation = frames[fId].transformation(jId);

                    //Get data
                    typename Affine3<T>::LinearMatrixType rotMatrix, scalMatrix;
                    animationTransformation.computeRotationScaling(&rotMatrix, &scalMatrix);
                    Vector3<T> animationTraVec(animationTransformation.translation());
                    Rotation3<T> animationRot(rotMatrix);
                    Vector3<T> animationScaVec(scalMatrix.diagonal());

                    //Get rotation transformation informations
                    SkeletonScalar angle = animationRot.angle();
                    Vector3<SkeletonScalar> axis = animationRot.axis();
                    axis = tRotation * axis;

                    //Get translation transformation informations
                    animationTraVec = tScaling * tRotation * animationTraVec;

                    //Set new transformation
                    animationTransformation.fromPositionOrientationScale(
                                animationTraVec,
                                Rotation3<SkeletonScalar>(angle, axis),
                                animationScaVec);
                }
            }
        }

        skeleton.setJointBindPose(jId, r);
    }

    #pragma omp parallel for
    for (Index vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (!mesh.isVertexDeleted(vId)) {
            T t = animationLinearBlendingSkinningVertex(skinningWeights, transformations, vId);

            const Point3d& p = mesh.vertexPoint(vId);
            mesh.setVertexPoint(vId, t * p);

            if (mesh.hasVertexNormals()) {
                const Vector3d& n = mesh.vertexNormal(vId);
                mesh.setVertexNormal(vId, t.rotation() * n);
            }
        }
    }

    if (preserveAnimations) {
        std::vector<SkeletonTransformation> newLocalBindPose = skeletonLocalBindPose(skeleton);

        animations = globalAnimations;

        for (Index i = 0; i < animations.size(); ++i) {
            A& animation = animations[i];

            animationLocalFromGlobal(skeleton, animation);

            #pragma omp parallel for
            for (nvl::Index fId = 0; fId < animation.keyframeNumber(); ++fId) {
                for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
                    AnimationTransformation& t = animation.keyframe(fId).transformation(jId);

                    t = newLocalBindPose[jId].inverse() * t;
                }
            }
        }

        if (!keepRotationInBindPose) {
            modelRemoveRotationInBindPose(skeleton, animations);
        }
    }
}

template<class M, class T>
void modelDeformLinearBlendingSkinning(
        M& model,
        const std::vector<T>& transformations,
        const bool keepRotationInBindPose,
        const bool preserveAnimations)
{
    return modelDeformLinearBlendingSkinning(model.mesh, model.skeleton, model.skinningWeights, model.animations, transformations, keepRotationInBindPose, preserveAnimations);
}

template<class M, class S, class W, class A, class T>
void modelDeformDualQuaternionSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<DualQuaternion<T>>& transformations,
        const bool keepRotationInBindPose,
        const bool preserveAnimations)
{
    typedef typename S::JointId JointId;
    typedef typename S::Scalar SkeletonScalar;
    typedef typename S::Transformation SkeletonTransformation;

    typedef typename A::Transformation AnimationTransformation;
    typedef typename A::Frame Frame;
    typedef typename A::FrameId FrameId;

    std::vector<A> globalAnimations;

    if (preserveAnimations) {
        std::vector<SkeletonTransformation> localBindPose = skeletonLocalBindPose(skeleton);

        globalAnimations.resize(animations.size());
        for (Index i = 0; i < animations.size(); ++i) {
            globalAnimations[i] = animations[i];

            #pragma omp parallel for
            for (nvl::Index fId = 0; fId < globalAnimations[i].keyframeNumber(); ++fId) {
                for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
                    AnimationTransformation& t = globalAnimations[i].keyframe(fId).transformation(jId);

                    t = localBindPose[jId] * t;
                }
            }

            animationGlobalFromLocal(skeleton, globalAnimations[i]);
        }
    }

    #pragma omp parallel for
    for (Index jId = 0; jId < skeleton.jointNumber(); ++jId) {
        const DualQuaterniond& dq = transformations[jId];

        SkeletonTransformation r;

        if (keepRotationInBindPose || preserveAnimations) {
            r = dq.affineTransformation() * skeleton.jointBindPose(jId);
        }
        else {
            Point3d p = skeleton.jointBindPose(jId) * Point3<SkeletonScalar>::Zero();
            p = dq * p;

            Translation3d jointTranslation(p);

            r = jointTranslation * SkeletonTransformation::Identity();

            const Quaternion<T> dqRotation(dq.rotation());
            for (A& animation : animations) {
                std::vector<Frame>& frames = animation.keyframes();
                for (FrameId fId = 0; fId < frames.size(); ++fId) {
                    AnimationTransformation& animationTransformation = frames[fId].transformation(jId);

                    //Get data
                    typename Affine3<T>::LinearMatrixType rotMatrix, scalMatrix;
                    animationTransformation.computeRotationScaling(&rotMatrix, &scalMatrix);
                    Vector3<T> animationTraVec(animationTransformation.translation());
                    Rotation3<T> animationRot(rotMatrix);
                    Vector3<T> animationScaVec(scalMatrix.diagonal());

                    //Get rotation transformation informations
                    T angle = animationRot.angle();
                    Vector3<T> axis = animationRot.axis();
                    axis = dqRotation * axis;

                    //Get translation transformation informations
                    animationTraVec = dqRotation * animationTraVec;

                    //Set new transformation
                    animationTransformation.fromPositionOrientationScale(
                                animationTraVec,
                                Rotation3<T>(angle, axis),
                                animationScaVec);
                }
            }
        }

        skeleton.setJointBindPose(jId, r);
    }

    #pragma omp parallel for
    for (Index vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (!mesh.isVertexDeleted(vId)) {
            DualQuaterniond dq = animationDualQuaternionSkinningVertex(skinningWeights, transformations, vId);

            const Point3d& p = mesh.vertexPoint(vId);
            mesh.setVertexPoint(vId, dq * p);

            if (mesh.hasVertexNormals()) {
                const Vector3d& n = mesh.vertexNormal(vId);
                mesh.setVertexNormal(vId, dq.rotation() * n);
            }
        }
    }

    if (preserveAnimations) {
        std::vector<SkeletonTransformation> newLocalBindPose = skeletonLocalBindPose(skeleton);

        animations = globalAnimations;

        for (Index i = 0; i < animations.size(); ++i) {
            A& animation = animations[i];

            animationLocalFromGlobal(skeleton, animation);

            #pragma omp parallel for
            for (nvl::Index fId = 0; fId < animation.keyframeNumber(); ++fId) {
                for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
                    AnimationTransformation& t = animation.keyframe(fId).transformation(jId);

                    t = newLocalBindPose[jId].inverse() * t;
                }
            }
        }

        if (!keepRotationInBindPose) {
            modelRemoveRotationInBindPose(skeleton, animations);
        }
    }
}

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<DualQuaternion<T>>& transformations,
        const bool keepRotationInBindPose,
        const bool preserveAnimations)
{
    return modelDeformDualQuaternionSkinning(model.mesh, model.skeleton, model.skinningWeights, model.animations, transformations, keepRotationInBindPose, preserveAnimations);
}

template<class M, class S, class W, class A, class T>
void modelDeformDualQuaternionSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<T>& transformations,
        const bool keepRotationInBindPose,
        const bool preserveAnimations)
{

    typedef typename T::Scalar Scalar;

    std::vector<DualQuaternion<Scalar>> dualQuaternionTransformations(transformations.size());

    #pragma omp parallel for
    for (Index i = 0; i < transformations.size(); ++i) {
        dualQuaternionTransformations[i] = DualQuaternion<Scalar>(Quaternion<Scalar>(transformations[i].rotation()), transformations[i].translation());
    }

    modelDeformDualQuaternionSkinning(mesh, skeleton, skinningWeights, animations, dualQuaternionTransformations, keepRotationInBindPose, preserveAnimations);
}

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<T>& transformations,
        const bool keepRotationInBindPose,
        const bool preserveAnimations)
{
    return modelDeformDualQuaternionSkinning(model.mesh, model.skeleton, model.skinningWeights, model.animations, transformations, keepRotationInBindPose, preserveAnimations);
}

}
