#include "animation_transformations.h"

#include <vector>

namespace nvl {

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Affine3<T>& transformation)
{
    Matrix33<T> rotMatrix, scalMatrix;
    transformation.computeScalingRotation(&scalMatrix, &rotMatrix);

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
            Vector3<T> jointTranslation(transformations[jId].translation());
            jointTranslation = transformation * jointTranslation;

            transformations[jId].fromPositionOrientationScale(
                jointTranslation,
                transformations[jId].rotation(),
                Vector3<T>(1.0, 1.0, 1.0));
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

            //Get rotation transformation informations
            Rotation3<T> animationRot(animationTransformation.rotation());
            T angle = animationRot.angle();
            Vector3<T> axis = animationRot.axis();
            axis = transformation * axis;

            //Get translation transformation informations
            Vector3<T> animationTra(animationTransformation.translation());
            animationTra = transformation * animationTra;

            //Set new transformation
            animationTransformation.fromPositionOrientationScale(
                        animationTra,
                        Rotation3<T>(angle, axis),
                        Vector3<T>(1.0, 1.0, 1.0));
        }
    }
}

}
