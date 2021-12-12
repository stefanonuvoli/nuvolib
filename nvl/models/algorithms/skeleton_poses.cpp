#include "skeleton_poses.h"

namespace nvl {

/* ----------------------- GLOBAL AND LOCAL BIND POSES ----------------------- */

template<class S>
std::vector<typename S::Transformation> skeletonGlobalBindPose(
        const S& skeleton)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Transformation Transformation;

    //Get bind pose transformations
    std::vector<Transformation> bindPose(skeleton.jointNumber());

    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        bindPose[jId] = skeleton.jointBindPose(jId);
    }

    return bindPose;
}


template<class S>
std::vector<typename S::Transformation> skeletonLocalBindPose(
        const S& skeleton)
{
    typedef S Skeleton;
    typedef typename Skeleton::Transformation Transformation;

    //Get bind pose transformations
    std::vector<Transformation> localBindPose = skeletonGlobalBindPose(skeleton);

    skeletonPoseLocalFromGlobal(skeleton, localBindPose);

    return localBindPose;
}


/* ----------------------- DEFORMATION FROM LOCAL ----------------------- */

template<class S, class T>
void skeletonPoseDeformationFromLocal(
        const S& skeleton,
        std::vector<T>& transformations)
{
    skeletonPoseDeformationFromLocal(skeleton, transformations, skeletonLocalBindPose(skeleton));
}

template<class S, class T, class B>
void skeletonPoseDeformationFromLocal(
        const S& skeleton,
        std::vector<T>& transformations,
        const std::vector<B>& localBindPose)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    //Add local bind pose transformations
    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        transformations[jId] = localBindPose[jId] * transformations[jId];
    }

    //Compute global frames
    skeletonPoseGlobalFromLocal(skeleton, transformations);

    //Add inverse bind pose transformations
    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        transformations[jId] = transformations[jId] * skeleton.jointBindPose(jId).inverse();
    }
}



/* ----------------------- LOCAL FROM GLOBAL ----------------------- */

template<class S, class T>
void skeletonPoseLocalFromGlobal(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        skeletonPoseLocalFromGlobal(skeleton, rootId, transformations);
    }
}

template<class S, class T>
void skeletonPoseLocalFromGlobal(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef T Transformation;
    typedef typename Skeleton::JointId JointId;

    const Transformation& currentTransformation = transformations[currentId];

    #pragma omp parallel for
    for (Index i = 0; i < skeleton.childrenNumber(currentId); ++i) {
        JointId childId = skeleton.child(currentId, i);

        skeletonPoseLocalFromGlobal(skeleton, childId, transformations);

        transformations[childId] = currentTransformation.inverse() * transformations[childId];
    }
}



/* ----------------------- GLOBAL FROM LOCAL ----------------------- */

template<class S, class T>
void skeletonPoseGlobalFromLocal(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    for (const JointId rootId : skeleton.roots()) {
        skeletonPoseGlobalFromLocal(skeleton, rootId, transformations);
    }
}

template<class S, class T>
void skeletonPoseGlobalFromLocal(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef T Transformation;
    typedef typename Skeleton::JointId JointId;

    const Transformation& currentTransformation = transformations[currentId];

    #pragma omp parallel for
    for (Index i = 0; i < skeleton.childrenNumber(currentId); ++i) {
        JointId childId = skeleton.child(currentId, i);

        transformations[childId] = currentTransformation * transformations[childId];

        skeletonPoseGlobalFromLocal(skeleton, childId, transformations);
    }
}

}
