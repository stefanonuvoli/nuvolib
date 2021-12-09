#ifndef NVL_MODELS_SKELETON_POSES_H
#define NVL_MODELS_SKELETON_POSES_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {


/* ----------------------- GLOBAL AND LOCAL BIND POSES ----------------------- */

template<class S>
std::vector<typename S::Transformation> skeletonBindPoseTransformations(
        const S& skeleton);

template<class S>
std::vector<typename S::Transformation> skeletonLocalBindPoseTransformations(
        const S& skeleton);

/* ----------------------- DEFORMATION FROM LOCAL ----------------------- */

template<class S, class T>
void skeletonPoseDeformationFromLocal(
        const S& skeleton,
        std::vector<T>& transformations);
template<class S, class T, class B>
void skeletonPoseDeformationFromLocal(
        const S& skeleton,
        std::vector<T>& transformations,
        const std::vector<B>& localBindPose);


/* ----------------------- LOCAL FROM GLOBAL ----------------------- */

template<class S, class T>
void skeletonPoseLocalFromGlobal(
        const S& skeleton,
        std::vector<T>& transformations);
template<class S, class T>
void skeletonPoseLocalFromGlobal(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations);


/* ----------------------- GLOBAL FROM LOCAL ----------------------- */

template<class S, class T>
void skeletonPoseGlobalFromLocal(
        const S& skeleton,
        std::vector<T>& transformations);
template<class S, class T>
void skeletonPoseGlobalFromLocal(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations);

}

#include "skeleton_poses.cpp"

#endif // NVL_MODELS_SKELETON_POSES_H
