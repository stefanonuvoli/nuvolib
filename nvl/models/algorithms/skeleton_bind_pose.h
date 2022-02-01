/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_SKELETON_BIND_POSE_H
#define NVL_MODELS_SKELETON_BIND_POSE_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

/* ----------------------- GLOBAL ----------------------- */

template<class S>
std::vector<typename S::Transformation> skeletonGlobalBindPose(
        const S& skeleton);

template<class S>
std::vector<typename S::Transformation> skeletonGlobalInverseBindPose(
        const S& skeleton);

template<class S>
void skeletonSetGlobalBindPose(
        S& skeleton,
        const std::vector<typename S::Transformation>& bindPose);



/* ----------------------- LOCAL ----------------------- */

template<class S>
std::vector<typename S::Transformation> skeletonLocalBindPose(
        const S& skeleton);

template<class S>
std::vector<typename S::Transformation> skeletonLocalInverseBindPose(
        const S& skeleton);

template<class S>
void skeletonSetLocalBindPose(
        S& skeleton,
        const std::vector<typename S::Transformation>& bindPose);

}

#include "skeleton_bind_pose.cpp"

#endif // NVL_MODELS_SKELETON_BIND_POSE_H
