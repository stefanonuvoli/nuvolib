#include "model_pose_propagation.h"

#include <nvl/math/interpolation.h>

namespace nvl {

/* ----------------------- DEFORMATION FROM LOCAL ----------------------- */

template<class M>
void modelDeformationFromLocal(
        M& model)
{
    typedef M Model;
    typedef typename Model::Animation Animation;

    for (Animation& a : model.animations) {
        animationDeformationFromLocal(model->skeleton, a);
    }
}

template<class S, class A>
void animationDeformationFromLocal(
        const S& skeleton,
        A& animation)
{
    typedef S Skeleton;

    animationFrameDeformationFromLocal(skeleton, animation, skeletonLocalBindPose(skeleton));
}

template<class S, class A, class B>
void animationDeformationFromLocal(
        const S& skeleton,
        A& animation,
        const std::vector<B>& localBindPose)
{
    animationFrameDeformationFromLocal(skeleton, animation, localBindPose);
}

template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        std::vector<F>& frames)
{
    typedef S Skeleton;

    animationFrameDeformationFromLocal(skeleton, frames, skeletonLocalBindPose(skeleton));
}

template<class S, class F, class B>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        std::vector<F>& frames,
        const std::vector<B>& localBindPose)
{
    typedef S Skeleton;

    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameDeformationFromLocal(skeleton, frames[fId], localBindPose);
    }
}

template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        F& frame)
{
    typedef S Skeleton;

    animationFrameDeformationFromLocal(skeleton, frame, skeletonLocalBindPose(skeleton));
}

template<class S, class F, class B>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        F& frame,
        const std::vector<B>& localBindPose)
{
    typedef S Skeleton;

    skeletonPoseDeformationFromLocal(skeleton, frame.transformations(), localBindPose);
}

template<class S, class T>
void skeletonPoseDeformationFromLocal(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;

    animationFrameDeformationFromLocal(skeleton, transformations, skeletonLocalBindPose(skeleton));
}

template<class S, class T, class B>
void skeletonPoseDeformationFromLocal(
        const S& skeleton,
        std::vector<T>& transformations,
        const std::vector<B>& localBindPose)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    //Add local bind pose transformations
    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        transformations[jId] = localBindPose[jId] * transformations[jId];
    }

    //Compute global frames
    skeletonPoseGlobalFromLocal(skeleton, transformations);

    //Add inverse bind pose transformations
    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        transformations[jId] = transformations[jId] * skeleton.jointBindPose(jId).inverse();
    }
}


/* ----------------------- LOCAL FROM GLOBAL ----------------------- */

template<class M>
void modelLocalFromGlobal(
        M& model)
{
    typedef M Model;
    typedef typename Model::Animation Animation;

    for (Animation& a : model.animations) {
        modelLocalFromGlobal(model, a);
    }
}

template<class S, class A>
void animationLocalFromGlobal(
        const S& skeleton,
        A& animation)
{
    animationLocalFromGlobal(skeleton, animation.keyframes());
}

template<class S, class F>
void animationFrameLocalFromGlobal(
        const S& skeleton,
        std::vector<F>& frames)
{
    typedef S Skeleton;
    typedef typename F::Transformation Transformation;

    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameLocalFromGlobal(skeleton, frames[fId]);
    }
}

template<class S, class F>
void animationFrameLocalFromGlobal(
        const S& skeleton,
        F& frame)
{
    typedef S Skeleton;

    skeletonPoseLocalFromGlobal(skeleton, frame.transformations());
}

template<class S, class T>
void skeletonPoseLocalFromGlobal(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        skeletonPoseLocalFromGlobal(skeleton, rootId, transformations);
    }
}

template<class S, class T>
void skeletonPoseLocalFromGlobal(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef T Transformation;
    typedef typename Skeleton::JointId JointId;

    const Transformation& currentTransformation = transformations[currentId];

    for (Index i = 0; i < skeleton.childrenNumber(currentId); ++i) {
        JointId childId = skeleton.child(currentId, i);

        skeletonPoseLocalFromGlobal(skeleton, childId, transformations);

        transformations[childId] = currentTransformation.inverse() * transformations[childId];
    }
}


/* ----------------------- GLOBAL FROM LOCAL ----------------------- */

template<class M>
void modelGlobalFromLocal(
        M& model)
{
    typedef M Model;
    typedef typename Model::Animation Animation;

    for (Animation& a : model.animations) {
        modelGlobalFromLocal(model, a);
    }
}

template<class S, class A>
void animationGlobalFromLocal(
        const S& skeleton,
        A& animation)
{
    animationGlobalFromLocal(skeleton, animation.keyframes());
}



template<class S, class F>
void animationFrameGlobalFromLocal(
        const S& skeleton,
        std::vector<F>& frames)
{
    typedef S Skeleton;
    typedef typename F::Transformation Transformation;

    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameGlobalFromLocal(skeleton, frames[fId]);
    }
}

template<class S, class F>
void animationFrameGlobalFromLocal(
        const S& skeleton,
        F& frame)
{
    typedef S Skeleton;
    skeletonPoseGlobalFromLocal(skeleton, frame.transformations());
}

template<class S, class T>
void skeletonPoseGlobalFromLocal(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        skeletonPoseGlobalFromLocal(skeleton, rootId, transformations);
    }
}

template<class S, class T>
void skeletonPoseGlobalFromLocal(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef T Transformation;
    typedef typename Skeleton::JointId JointId;

    const Transformation& currentTransformation = transformations[currentId];

    for (Index i = 0; i < skeleton.childrenNumber(currentId); ++i) {
        JointId childId = skeleton.child(currentId, i);

        transformations[childId] = currentTransformation * transformations[childId];

        skeletonPoseGlobalFromLocal(skeleton, childId, transformations);
    }
}



/* ----------------------- SKELETON UTILS ----------------------- */

template<class S>
std::vector<typename S::Transformation> skeletonGlobalBindPose(
        const S& skeleton)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Transformation Transformation;

    //Get bind pose transformations
    std::vector<Transformation> bindPose(skeleton.jointNumber());

    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        bindPose[jId] = skeleton.jointBindPose(jId);
    }

    return bindPose;
}

template<class S>
std::vector<typename S::Transformation> skeletonLocalBindPose(
        const S& skeleton)
{
    typedef S Skeleton;
    typedef typename Skeleton::Transformation Transformation;

    //Get bind pose transformations
    std::vector<Transformation> localBindPose = skeletonGlobalBindPose(skeleton);

    skeletonPoseLocalFromGlobal(skeleton, localBindPose);

    return localBindPose;
}

}
