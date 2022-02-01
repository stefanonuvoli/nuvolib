/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_SKELETON_TRANSFORMATIONS_H
#define NVL_MODELS_SKELETON_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

namespace nvl {

template<class Skeleton, class T>
void skeletonApplyTransformation(Skeleton& skeleton, const T& transformation);

template<class Skeleton, class T>
void skeletonApplyTransformation(Skeleton& skeleton, const std::vector<T>& transformations);

template<class Skeleton>
void skeletonSetRoot(Skeleton& skeleton, const typename Skeleton::JointId& jId);

}

#include "skeleton_transformations.cpp"

#endif // NVL_MODELS_SKELETON_TRANSFORMATIONS_H
