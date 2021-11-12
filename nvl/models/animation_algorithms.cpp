#include "animation_algorithms.h"

#include <nvl/math/interpolation.h>

namespace nvl {

template<class S, class F>
std::vector<F> animationComputeFinalTransformations(
        const S& skeleton,
        const std::vector<F>& frames)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Transformation Transformation;

    std::vector<F> finalFrames = frames;

    //Get local rest pose
    std::vector<Transformation> localRestPoses(skeleton.jointNumber());
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        localRestPoses[jId] = skeleton.joint(jId).restPose();
    }
    animationComputeLocalTransformations(skeleton, localRestPoses);

    //Add inverse rest pose transformations
    #pragma omp parallel for
    for (Index fId = 0; fId < finalFrames.size(); ++fId) {
        std::vector<Transformation>& transformations = finalFrames[fId].transformations();
        for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
            transformations[jId] = localRestPoses[jId] * transformations[jId];
        }
    }

    //Compute global frames
    animationComputeGlobalFrames(skeleton, finalFrames);

    //Add inverse rest pose transformations
    #pragma omp parallel for
    for (Index fId = 0; fId < finalFrames.size(); ++fId) {
        std::vector<Transformation>& transformations = finalFrames[fId].transformations();
        for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
            transformations[jId] = transformations[jId] * skeleton.joint(jId).restPose().inverse();
        }
    }

    return finalFrames;
}

template<class S, class F>
void animationComputeGlobalFrames(
        const S& skeleton,
        std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationComputeGlobalFrame(skeleton, frames[fId]);
    }
}

template<class S, class F>
void animationComputeGlobalFrame(
        const S& skeleton,
        F& frame)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        animationComputeGlobalFrame(skeleton, rootId, frame);
    }
}

template<class S, class F>
void animationComputeGlobalFrame(
        const S& skeleton,
        const typename S::JointId& currentId,
        F& frame)
{
    return animationComputeGlobalTransformations(skeleton, currentId, frame.transformations());
}

template<class S, class T>
void animationComputeGlobalTransformations(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        animationComputeGlobalTransformations(skeleton, rootId, transformations);
    }
}

template<class S, class T>
void animationComputeGlobalTransformations(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef T Transformation;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Joint Joint;

    const Transformation& currentTransformation = transformations[currentId];

    for (Index i = 0; i < skeleton.childrenNumber(currentId); ++i) {
        JointId childId = skeleton.child(currentId, i);

        transformations[childId] = currentTransformation * transformations[childId];

        animationComputeGlobalTransformations(skeleton, childId, transformations);
    }
}

template<class S, class F>
void animationComputeLocalFrames(
        const S& skeleton,
        std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationComputeLocalFrame(skeleton, frames[fId]);
    }
}

template<class S, class F>
void animationComputeLocalFrame(
        const S& skeleton,
        F& frame)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        animationComputeLocalFrame(skeleton, rootId, frame);
    }
}

template<class S, class F>
void animationComputeLocalFrame(
        const S& skeleton,
        const typename S::JointId& currentId,
        F& frame)
{
    animationComputeLocalTransformations(skeleton, currentId, frame.transformations());
}

template<class S, class T>
void animationComputeLocalTransformations(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        animationComputeLocalTransformations(skeleton, rootId, transformations);
    }
}

template<class S, class T>
void animationComputeLocalTransformations(
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

        animationComputeLocalTransformations(skeleton, childId, transformations);

        transformations[childId] = currentTransformation.inverse() * transformations[childId];
    }
}

template<class F>
void animationBlendFrameTransformations(
        std::vector<F>& animationFrames,
        const double& fps,
        const double& speed,
        const bool& keepKeyframes)
{
    typedef F Frame;
    typedef typename Frame::Transformation Transformation;

    assert(speed > 0.0);

    if (animationFrames.empty())
        return;

    const double timeStep = 1.0 / (fps / speed);

    //Blending
    std::vector<Frame> blendedAnimationFrames;
    blendedAnimationFrames.reserve(animationFrames.size());

    //Add first frame
    blendedAnimationFrames.push_back(animationFrames[0]);

    Index currentFrameId = 0;
    double currentTime = timeStep;
    while (currentFrameId < animationFrames.size() - 1) {
        const Frame& frame1 = animationFrames[currentFrameId];
        const double& time1 = frame1.time();
        const Frame& frame2 = animationFrames[currentFrameId+1];
        const double& time2 = frame2.time();

        assert(time1 < time2);
        while (time1 > currentTime) {
            currentTime += timeStep;
        }

        if (time2 <= currentTime) {
            currentFrameId++;
            if (keepKeyframes && currentFrameId < animationFrames.size() - 1) {
                blendedAnimationFrames.push_back(animationFrames[currentFrameId]);

                if (nvl::epsEqual(time2, currentTime, 0.01)) {
                    currentTime += timeStep;
                }
            }
        }
        else {
            const std::vector<Transformation>& transformations1 = frame1.transformations();
            const std::vector<Transformation>& transformations2 = frame2.transformations();
            const double alpha = (currentTime - time1) / (time2 - time1);

            Frame blendedFrame;
            blendedFrame.setTime(currentTime);

            std::vector<Transformation>& transformations = blendedFrame.transformations();
            transformations.resize(transformations1.size());
            for (Index jId = 0; jId < transformations1.size(); ++jId) {
                transformations[jId] = nvl::interpolateAffine(transformations1[jId], transformations2[jId], alpha);
            }

            blendedAnimationFrames.push_back(blendedFrame);

            currentTime += timeStep;
        }
    }

    for (Index i = 0; i < blendedAnimationFrames.size() - 1; ++i) {
        assert(blendedAnimationFrames[i].time() < blendedAnimationFrames[i+1].time());
    }

    //Add last frame
    blendedAnimationFrames.push_back(animationFrames[animationFrames.size() - 1]);

    animationFrames = blendedAnimationFrames;
}

}
