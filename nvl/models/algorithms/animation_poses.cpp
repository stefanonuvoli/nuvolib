#include "animation_poses.h"

#include <nvl/models/algorithms/skeleton_poses.h>

namespace nvl {

/* ----------------------- DEFORMATION FROM LOCAL ----------------------- */

template<class S, class A>
void animationDeformationFromLocal(
        const S& skeleton,
        A& animation)
{
    animationFrameDeformationFromLocal(skeleton, animation, skeletonLocalBindPose(skeleton));
}

template<class S, class A, class B>
void animationDeformationFromLocal(
        const S& skeleton,
        A& animation,
        const std::vector<B>& localBindPose)
{
    animationFrameDeformationFromLocal(skeleton, animation, localBindPose);
}

template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        std::vector<F>& frames)
{
    animationFrameDeformationFromLocal(skeleton, frames, skeletonLocalBindPose(skeleton));
}

template<class S, class F, class B>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        std::vector<F>& frames,
        const std::vector<B>& localBindPose)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameDeformationFromLocal(skeleton, frames[fId], localBindPose);
    }
}

template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        F& frame)
{
    animationFrameDeformationFromLocal(skeleton, frame, skeletonLocalBindPose(skeleton));
}

template<class S, class F, class B>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        F& frame,
        const std::vector<B>& localBindPose)
{
    skeletonPoseDeformationFromLocal(skeleton, frame.transformations(), localBindPose);
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
