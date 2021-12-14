#include "skeleton_poses.h"

namespace nvl {

/* ----------------------- INTERNAL FUNCTIONS DECLARATION ----------------------- */

namespace internal {

template<class S, class T>
void skeletonPoseGlobalFromLocalRecursive(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations);

template<class S, class T>
void skeletonPoseLocalFromGlobalRecursive(
        const S& skeleton,
        const typename S::JointId& currentId,
        std::vector<T>& transformations);
}

/* ----------------------- DEFORMATION FROM LOCAL AND GLOBAL ----------------------- */

template<class S, class T>
void skeletonPoseDeformationFromGlobal(
        const S& skeleton,
        std::vector<T>& transformations)
{
    typedef S Skeleton;
    typedef typename Skeleton::JointId JointId;

    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        transformations[jId] = transformations[jId] * skeleton.jointBindPose(jId).inverse();
    }
}

template<class S, class T>
void skeletonPoseDeformationFromLocal(
        const S& skeleton,
        std::vector<T>& transformations)
{
    skeletonPoseGlobalFromLocal(skeleton, transformations);
    skeletonPoseDeformationFromGlobal(skeleton, transformations);
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
        internal::skeletonPoseLocalFromGlobalRecursive(skeleton, rootId, transformations);
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
        internal::skeletonPoseGlobalFromLocalRecursive(skeleton, rootId, transformations);
    }
}


/* ----------------------- INTERNAL FUNCTIONS ----------------------- */

namespace internal {

template<class S, class T>
void skeletonPoseGlobalFromLocalRecursive(
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

        skeletonPoseGlobalFromLocalRecursive(skeleton, childId, transformations);
    }
}

template<class S, class T>
void skeletonPoseLocalFromGlobalRecursive(
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

        skeletonPoseLocalFromGlobalRecursive(skeleton, childId, transformations);

        transformations[childId] = currentTransformation.inverse() * transformations[childId];
    }
}

}

}
