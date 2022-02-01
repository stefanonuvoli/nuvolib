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

template<class S, class A>
void animationDeformationFromGlobal(
        const S& skeleton,
        A& animation)
{
    animationFrameDeformationFromGlobal(skeleton, animation);
}

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

template<class S, class F>
void animationFrameDeformationFromGlobal(
        const S& skeleton,
        F& frame)
{
    skeletonPoseDeformationFromGlobal(skeleton, frame.transformations());
}

template<class S, class A>
void animationDeformationFromLocal(
        const S& skeleton,
        A& animation)
{
    animationFrameDeformationFromLocal(skeleton, animation);
}

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

template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        F& frame)
{
    skeletonPoseDeformationFromLocal(skeleton, frame.transformations());
}


/* ----------------------- LOCAL FROM GLOBAL ----------------------- */

template<class S, class A>
void animationLocalFromGlobal(
        const S& skeleton,
        A& animation)
{
    animationFrameLocalFromGlobal(skeleton, animation.keyframes());
}

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

template<class S, class F>
void animationFrameLocalFromGlobal(
        const S& skeleton,
        F& frame)
{
    skeletonPoseLocalFromGlobal(skeleton, frame.transformations());
}



/* ----------------------- GLOBAL FROM LOCAL ----------------------- */

template<class S, class A>
void animationGlobalFromLocal(
        const S& skeleton,
        A& animation)
{
    animationFrameGlobalFromLocal(skeleton, animation.keyframes());
}

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

template<class S, class F>
void animationFrameGlobalFromLocal(
        const S& skeleton,
        F& frame)
{
    skeletonPoseGlobalFromLocal(skeleton, frame.transformations());
}


}
