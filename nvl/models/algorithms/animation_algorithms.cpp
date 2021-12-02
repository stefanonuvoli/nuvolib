#include "animation_algorithms.h"

#include <nvl/math/interpolation.h>

namespace nvl {

template<class S, class F>
void animationComputeFinalFromLocalFrames(
        const S& skeleton,
        std::vector<F>& frames)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Transformation Transformation;

    //Get local bind pose
    std::vector<Transformation> localBindPoses(skeleton.jointNumber());
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        localBindPoses[jId] = skeleton.jointBindPose(jId);
    }
    animationLocalFromGlobalTransformations(skeleton, localBindPoses);

    //Add local bind pose transformations
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        std::vector<Transformation>& transformations = frames[fId].transformations();
        for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
            transformations[jId] = localBindPoses[jId] * transformations[jId];
        }
    }

    //Compute global frames
    animationGlobalFromLocalFrames(skeleton, frames);

    //Add inverse bind pose transformations
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        std::vector<Transformation>& transformations = frames[fId].transformations();
        for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
            transformations[jId] = transformations[jId] * skeleton.jointBindPose(jId);
        }
    }
}

template<class S, class F>
void animationGlobalFromLocalFrames(
        const S& skeleton,
        std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationGlobalFromLocalFrame(skeleton, frames[fId]);
    }
}

template<class S, class F>
void animationGlobalFromLocalFrame(
        const S& skeleton,
        F& frame)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        animationGlobalFromLocalFrame(skeleton, rootId, frame);
    }
}

template<class S, class F>
void animationGlobalFromLocalFrame(
        const S& skeleton,
        const typename S::JointId& currentId,
        F& frame)
{
    return animationGlobalFromLocalTransformations(skeleton, currentId, frame.transformations());
}

template<class S, class T>
void animationGlobalFromLocalTransformations(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        animationGlobalFromLocalTransformations(skeleton, rootId, transformations);
    }
}

template<class S, class T>
void animationGlobalFromLocalTransformations(
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

        animationGlobalFromLocalTransformations(skeleton, childId, transformations);
    }
}

template<class S, class F>
void animationLocalFromGlobalFrames(
        const S& skeleton,
        std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationLocalFromGlobalFrame(skeleton, frames[fId]);
    }
}

template<class S, class F>
void animationLocalFromGlobalFrame(
        const S& skeleton,
        F& frame)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        animationLocalFromGlobalFrame(skeleton, rootId, frame);
    }
}

template<class S, class F>
void animationLocalFromGlobalFrame(
        const S& skeleton,
        const typename S::JointId& currentId,
        F& frame)
{
    animationLocalFromGlobalTransformations(skeleton, currentId, frame.transformations());
}

template<class S, class T>
void animationLocalFromGlobalTransformations(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        animationLocalFromGlobalTransformations(skeleton, rootId, transformations);
    }
}

template<class S, class T>
void animationLocalFromGlobalTransformations(
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

        animationLocalFromGlobalTransformations(skeleton, childId, transformations);

        transformations[childId] = currentTransformation.inverse() * transformations[childId];
    }
}

}
