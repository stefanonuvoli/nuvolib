#ifndef NVL_MODELS_SKELETON_TRANSFORMATIONS_H
#define NVL_MODELS_SKELETON_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>
#include <nvl/math/transformations.h>

namespace nvl {

template<class Skeleton, class T>
void skeletonApplyTransformation(Skeleton& skeleton, const T& transformation, const bool onlyTranslation = true);

}

#include "skeleton_transformations.cpp"

#endif // NVL_MODELS_SKELETON_TRANSFORMATIONS_H
