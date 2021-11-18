#ifndef NVL_MODELS_ANIMATION_TRANSFORMATIONS_H
#define NVL_MODELS_ANIMATION_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

#include <nvl/math/transformations.h>

namespace nvl {

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Affine3<T>& transformation);

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Scaling3<T>& transformation);

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Quaternion<T>& transformation);

template<class Skeleton, class Animation, class T>
void animationApplyTransformation(Skeleton& skeleton, Animation& animation, const Rotation3<T>& transformation);

}

#include "animation_transformations.cpp"

#endif // NVL_MODELS_ANIMATION_TRANSFORMATIONS_H
