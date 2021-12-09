#ifndef NVL_MODELS_MODEL_POSE_PROPAGATION_H
#define NVL_MODELS_MODEL_POSE_PROPAGATION_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

/* ----------------------- DEFORMATION FROM LOCAL ----------------------- */

template<class M>
void modelDeformationFromLocal(
        M& model);

template<class S, class A>
void animationDeformationFromLocal(
        const S& skeleton,
        A& animation);
template<class S, class A, class B>
void animationDeformationFromLocal(
        const S& skeleton,
        A& animation,
        const std::vector<B>& localBindPose);

template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        std::vector<F>& frames);
template<class S, class F, class B>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        std::vector<F>& frames,
        const std::vector<B>& localBindPose);

template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        F& frame);
template<class S, class F, class B>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        F& frame,
        const std::vector<B>& localBindPose);

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

template<class M>
void modelLocalFromGlobal(
        M& model);

template<class S, class A>
void animationLocalFromGlobal(
        const S& skeleton,
        A& animation);
template<class S, class F>
void animationFrameLocalFromGlobal(
        const S& skeleton,
        std::vector<F>& frames);

template<class S, class F>
void animationFrameLocalFromGlobal(
        const S& skeleton,
        F& frame);

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

template<class M>
void modelGlobalFromLocal(
        M& model);

template<class S, class A>
void animationGlobalFromLocal(
        const S& skeleton,
        A& animation);
template<class S, class F>
void animationFrameGlobalFromLocal(
        const S& skeleton,
        std::vector<F>& frames);

template<class S, class F>
void animationFrameGlobalFromLocal(
        const S& skeleton,
        F& frame);

template<class S, class T>
void skeletonPoseGlobalFromLocal(
        const S& skeleton,
        std::vector<T>& transformations);
template<class S, class T>
void skeletonPoseGlobalFromLocal(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations);



/* ----------------------- SKELETON UTILS ----------------------- */

template<class S>
std::vector<typename S::Transformation> skeletonBindPoseTransformations(
        const S& skeleton);

template<class S>
std::vector<typename S::Transformation> skeletonLocalBindPoseTransformations(
        const S& skeleton);

}

#include "model_pose_propagation.cpp"

#endif // NVL_MODELS_MODEL_POSE_PROPAGATION_H
