#ifndef NVL_MODELS_ANIMATION_TRANSFORMATIONS_H
#define NVL_MODELS_ANIMATION_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

#include <nvl/math/transformations.h>

namespace nvl {

/* ----------------------- GEOMETRICAL TRANSFORMATIONS ----------------------- */

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Affine3<T>& transformation);

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Scaling3<T>& transformation);

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Quaternion<T>& transformation);

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Rotation3<T>& transformation);


/* ----------------------- TIME TRANSFORMATIONS ----------------------- */

template<class Animation>
void animationChangeDuration(Animation& animation, const double& duration);

template<class Frame>
void animationFrameChangeDuration(std::vector<Frame>& frames, const double& duration);

}

#include "animation_transformations.cpp"

#endif // NVL_MODELS_ANIMATION_TRANSFORMATIONS_H
