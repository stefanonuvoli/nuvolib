/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_SKELETON_POSES_H
#define NVL_MODELS_SKELETON_POSES_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

/* ----------------------- DEFORMATION FROM LOCAL AND GLOBAL ----------------------- */

template<class S, class T>
void skeletonPoseDeformationFromGlobal(
        const S& skeleton,
        std::vector<T>& transformations);
template<class S, class T>
void skeletonPoseDeformationFromLocal(
        const S& skeleton,
        std::vector<T>& transformations);


/* ----------------------- LOCAL FROM GLOBAL ----------------------- */

template<class S, class T>
void skeletonPoseLocalFromGlobal(
        const S& skeleton,
        std::vector<T>& transformations);


/* ----------------------- GLOBAL FROM LOCAL ----------------------- */

template<class S, class T>
void skeletonPoseGlobalFromLocal(
        const S& skeleton,
        std::vector<T>& transformations);

}

#include "skeleton_poses.cpp"

#endif // NVL_MODELS_SKELETON_POSES_H
