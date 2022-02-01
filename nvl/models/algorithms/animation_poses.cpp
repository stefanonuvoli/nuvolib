/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "animation_poses.h"

#include <nvl/models/algorithms/skeleton_poses.h>

namespace nvl {

/* ----------------------- DEFORMATION FROM LOCAL AND GLOBAL ----------------------- */

/**
 * @brief Get animation deformation from animation with global transformations
 * @param skeleton Skeleton
 * @param animation Animation with global transformations
 */
template<class S, class A>
void animationDeformationFromGlobal(
        const S& skeleton,
        A& animation)
{
    animationFrameDeformationFromGlobal(skeleton, animation);
}

/**
 * @brief Get animation deformation from animation frames with global transformations
 * @param skeleton Skeleton
 * @param frames Animation frames with global transformations
 */
template<class S, class F>
void animationFrameDeformationFromGlobal(
        const S& skeleton,
        std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameDeformationFromGlobal(skeleton, frames[fId]);
    }
}

/**
 * @brief Get animation deformation from animation frame with global transformations
 * @param skeleton Skeleton
 * @param frame Animation frame with global transformations
 */
template<class S, class F>
void animationFrameDeformationFromGlobal(
        const S& skeleton,
        F& frame)
{
    skeletonPoseDeformationFromGlobal(skeleton, frame.transformations());
}

/**
 * @brief Get animation deformation from animation with local transformations
 * @param skeleton Skeleton
 * @param animation Animation with local transformations
 */
template<class S, class A>
void animationDeformationFromLocal(
        const S& skeleton,
        A& animation)
{
    animationFrameDeformationFromLocal(skeleton, animation);
}

/**
 * @brief Get animation deformation from animation frames with local transformations
 * @param skeleton Skeleton
 * @param frames Animation frames with local transformations
 */
template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameDeformationFromLocal(skeleton, frames[fId]);
    }
}

/**
 * @brief Get animation deformation from animation frame with local transformations
 * @param skeleton Skeleton
 * @param frame Animation frame with local transformations
 */
template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        F& frame)
{
    skeletonPoseDeformationFromLocal(skeleton, frame.transformations());
}


/* ----------------------- LOCAL FROM GLOBAL ----------------------- */

/**
 * @brief Get animation with local transformation from animation with global transformations
 * @param skeleton Skeleton
 * @param frame Animation with global transformations
 */
template<class S, class A>
void animationLocalFromGlobal(
        const S& skeleton,
        A& animation)
{
    animationFrameLocalFromGlobal(skeleton, animation.keyframes());
}

/**
 * @brief Get animation frames with local transformation from animation frames with global transformations
 * @param skeleton Skeleton
 * @param frame Animation frames with global transformations
 */
template<class S, class F>
void animationFrameLocalFromGlobal(
        const S& skeleton,
        std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameLocalFromGlobal(skeleton, frames[fId]);
    }
}

/**
 * @brief Get animation frames with local transformation from animation frame with global transformations
 * @param skeleton Skeleton
 * @param frame Animation frame with global transformations
 */
template<class S, class F>
void animationFrameLocalFromGlobal(
        const S& skeleton,
        F& frame)
{
    skeletonPoseLocalFromGlobal(skeleton, frame.transformations());
}



/* ----------------------- GLOBAL FROM LOCAL ----------------------- */

/**
 * @brief Get animation with global transformation from animation with local transformations
 * @param skeleton Skeleton
 * @param frame Animation with local transformations
 */
template<class S, class A>
void animationGlobalFromLocal(
        const S& skeleton,
        A& animation)
{
    animationFrameGlobalFromLocal(skeleton, animation.keyframes());
}

/**
 * @brief Get animation frames with global transformation from animation frames with local transformations
 * @param skeleton Skeleton
 * @param frame Animation frames with local transformations
 */
template<class S, class F>
void animationFrameGlobalFromLocal(
        const S& skeleton,
        std::vector<F>& frames)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameGlobalFromLocal(skeleton, frames[fId]);
    }
}

/**
 * @brief Get animation frames with global transformation from animation frame with local transformations
 * @param skeleton Skeleton
 * @param frame Animation frame with local transformations
 */
template<class S, class F>
void animationFrameGlobalFromLocal(
        const S& skeleton,
        F& frame)
{
    skeletonPoseGlobalFromLocal(skeleton, frame.transformations());
}


}
