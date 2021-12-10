#include "model_deformation.h"

namespace nvl {

template<class M, class S, class W, class A, class T>
void modelDeformLinearBlendingSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<T>& transformations,
        const bool useBindPoseRotation)
{
    typedef typename S::Scalar SkeletonScalar;
    typedef typename S::Transformation SkeletonTransformation;

    typedef typename A::Transformation AnimationTransformation;
    typedef typename A::Frame Frame;
    typedef typename A::FrameId FrameId;

    #pragma omp parallel for
    for (Index jId = 0; jId < skeleton.jointNumber(); ++jId) {
        const T& t = transformations[jId];

        SkeletonTransformation r;

        if (useBindPoseRotation) {
            r = t * skeleton.jointBindPose(jId);
        }
        else {
            Point3d p = skeleton.jointBindPose(jId) * Point3<SkeletonScalar>::Zero();
            p = t * p;

            Translation3d jointTranslation(p);

            r = jointTranslation * SkeletonTransformation::Identity();

            Matrix33<SkeletonScalar> rotMatrix, scalMatrix;
            t.computeScalingRotation(&scalMatrix, &rotMatrix);

            const Quaternion<SkeletonScalar> tRotation(rotMatrix);
            const Scaling3<SkeletonScalar> tScaling(scalMatrix.diagonal());

            for (A& animation : animations) {
                std::vector<Frame>& frames = animation.keyframes();
                for (FrameId fId = 0; fId < frames.size(); ++fId) {
                    AnimationTransformation& animationTransformation = frames[fId].transformation(jId);

                    //Get rotation transformation informations
                    Rotation3<SkeletonScalar> animationRot(animationTransformation.rotation());
                    SkeletonScalar angle = animationRot.angle();
                    Vector3<SkeletonScalar> axis = animationRot.axis();
                    axis = tRotation * axis;

                    //Get translation transformation informations
                    Vector3<SkeletonScalar> animationTra(animationTransformation.translation());
                    animationTra = tScaling * tRotation * animationTra;

                    //Set new transformation
                    animationTransformation.fromPositionOrientationScale(
                                animationTra,
                                Rotation3<SkeletonScalar>(angle, axis),
                                Vector3<SkeletonScalar>(1.0, 1.0, 1.0));
                }
            }
        }

        skeleton.joint(jId).setBindPose(r);
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
}

template<class M, class T>
void modelDeformLinearBlendingSkinning(
        M& model,
        const std::vector<T>& transformations,
        const bool useBindPoseRotation)
{
    return modelDeformLinearBlendingSkinning(model.mesh, model.skeleton, model.skinningWeights, model.animations, transformations, useBindPoseRotation);
}

template<class M, class S, class W, class A, class T>
void modelDeformDualQuaternionSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<DualQuaternion<T>>& transformations,
        const bool useBindPoseRotation)
{
    typedef typename S::Scalar SkeletonScalar;
    typedef typename S::Transformation SkeletonTransformation;

    typedef typename A::Transformation AnimationTransformation;
    typedef typename A::Frame Frame;
    typedef typename A::FrameId FrameId;

    #pragma omp parallel for
    for (Index jId = 0; jId < skeleton.jointNumber(); ++jId) {
        const DualQuaterniond& dq = transformations[jId];

        SkeletonTransformation r;

        if (useBindPoseRotation) {
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

                    //Get rotation transformation informations
                    Rotation3<T> animationRot(animationTransformation.rotation());
                    T angle = animationRot.angle();
                    Vector3<T> axis = animationRot.axis();
                    axis = dqRotation * axis;

                    //Get translation transformation informations
                    Vector3<T> animationTra(animationTransformation.translation());
                    animationTra = dqRotation * animationTra;

                    //Set new transformation
                    animationTransformation.fromPositionOrientationScale(
                                animationTra,
                                Rotation3<T>(angle, axis),
                                Vector3<T>(1.0, 1.0, 1.0));
                }
            }
        }

        skeleton.joint(jId).setBindPose(r);
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
}

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<DualQuaternion<T>>& transformations,
        const bool useBindPoseRotation)
{
    return modelDeformDualQuaternionSkinning(model.mesh, model.skeleton, model.skinningWeights, model.animations, transformations, useBindPoseRotation);
}

template<class M, class S, class W, class A, class T>
void modelDeformDualQuaternionSkinning(
        M& mesh,
        S& skeleton,
        const W& skinningWeights,
        std::vector<A>& animations,
        const std::vector<T>& transformations,
        const bool useBindPoseRotation)
{

    typedef typename T::Scalar Scalar;

    std::vector<DualQuaternion<Scalar>> dualQuaternionTransformations(transformations.size());

    #pragma omp parallel for
    for (Index i = 0; i < transformations.size(); ++i) {
        dualQuaternionTransformations[i] = DualQuaternion<Scalar>(Quaternion<Scalar>(transformations[i].rotation()), transformations[i].translation());
    }

    modelDeformDualQuaternionSkinning(mesh, skeleton, skinningWeights, animations, dualQuaternionTransformations, useBindPoseRotation);
}

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<T>& transformations,
        const bool useBindPoseRotation)
{
    return modelDeformDualQuaternionSkinning(model.mesh, model.skeleton, model.skinningWeights, model.animations, transformations, useBindPoseRotation);
}

}
