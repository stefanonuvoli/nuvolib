#include "animation_transformations.h"

#include <vector>

namespace nvl {

/* ----------------------- GEOMETRICAL TRANSFORMATIONS ----------------------- */

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Affine3<T>& transformation)
{
    typename Affine3<T>::LinearMatrixType rotMatrix, scalMatrix;
    transformation.computeRotationScaling(&rotMatrix, &scalMatrix);

    const Quaternion<T> rot(rotMatrix);
    const Scaling3<T> sca(scalMatrix.diagonal());

    animationApplyTransformation(skeleton, animation, sca);
    animationApplyTransformation(skeleton, animation, rot);
}

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Scaling3<T>& transformation)
{
    typedef typename Animation::Frame Frame;
    typedef typename Animation::FrameId FrameId;
    typedef typename Animation::Transformation Transformation;
    typedef typename Skeleton::JointId JointId;

    #pragma omp parallel for
    for (FrameId fId = 0; fId < animation.keyframeNumber(); ++fId) {
        Frame& frame = animation.keyframe(fId);

        std::vector<Transformation>& transformations = frame.transformations();
        assert(!transformations.empty());

        #pragma omp parallel for
        for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
            //Get data
            typename Affine3<T>::LinearMatrixType rotMatrix, scalMatrix;
            transformations[jId].computeRotationScaling(&rotMatrix, &scalMatrix);
            Vector3<T> jointTraVec(transformations[jId].translation());
            Rotation3<T> jointRot(rotMatrix);
            Vector3<T> jointScaVec(scalMatrix.diagonal());

            jointTraVec = transformation * jointTraVec;

            transformations[jId].fromPositionOrientationScale(
                jointTraVec,
                jointRot,
                jointScaVec);
        }
    }
}

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Rotation3<T>& transformation)
{
    Quaternion<T> quaternion(transformation);
    return animationApplyTransformation(skeleton, animation, quaternion);
}

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Quaternion<T>& transformation)
{
    typedef typename Animation::Frame Frame;
    typedef typename Animation::FrameId FrameId;
    typedef typename Animation::Transformation Transformation;

    #pragma omp parallel for
    for (FrameId fId = 0; fId < animation.keyframeNumber(); ++fId) {
        Frame& frame = animation.keyframe(fId);

        std::vector<Transformation>& transformations = frame.transformations();
        assert(!transformations.empty());

        #pragma omp parallel for
        for (Index jId = 0; jId < skeleton.jointNumber(); ++jId) {
            Transformation& animationTransformation = transformations[jId];

            //Get data
            typename Affine3<T>::LinearMatrixType rotMatrix, scalMatrix;
            animationTransformation.computeRotationScaling(&rotMatrix, &scalMatrix);
            Vector3<T> animationTraVec(animationTransformation.translation());
            Rotation3<T> animationRot(rotMatrix);
            Vector3<T> animationScaVec(scalMatrix.diagonal());

            //Get rotation transformation informations
            T angle = animationRot.angle();
            Vector3<T> axis = animationRot.axis();
            axis = transformation * axis;

            //Get translation transformation informations
            animationTraVec = transformation * animationTraVec;

            //Set new transformation
            animationTransformation.fromPositionOrientationScale(
                        animationTraVec,
                        Rotation3<T>(angle, axis),
                        animationScaVec);
        }
    }
}



/* ----------------------- TIME TRANSFORMATIONS ----------------------- */

template<class Animation>
void animationChangeDuration(Animation& animation, const double& duration)
{
    animationChangeDuration(animation.keyframes(), duration);
}

template<class Frame>
void animationFrameChangeDuration(std::vector<Frame>& frames, const double& duration)
{
    if (frames.empty())
        return;

    double oldDuration = frames[frames.size() - 1].time();

    double scaleFactor = duration / oldDuration;

    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        Frame& frame = frames[fId];
        frame.time() *= scaleFactor;
    }
}

}
