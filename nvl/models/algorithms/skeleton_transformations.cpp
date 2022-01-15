#include "skeleton_transformations.h"

#include <vector>

#include <nvl/math/transformations.h>

namespace nvl {

template<class Skeleton, class T>
void skeletonApplyTransformation(Skeleton& skeleton, const T& transformation)
{
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Joint Joint;

    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        Joint& joint = skeleton.joint(jId);

        joint.setBindPose(transformation * joint.bindPose());
    }
}

template<class Skeleton, class T>
void skeletonApplyTransformation(Skeleton& skeleton, const std::vector<T>& transformations)
{
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Joint Joint;

    #pragma omp parallel for
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        Joint& joint = skeleton.joint(jId);

        joint.setBindPose(transformations[jId] * joint.bindPose());
    }
}

template<class Skeleton>
void skeletonSetRoot(Skeleton& skeleton, const typename Skeleton::JointId& jId)
{
    typedef typename Skeleton::JointId JointId;

    Skeleton skeletonCopy = skeleton;

    JointId lastJoint = NULL_ID;
    JointId currentJoint = jId;

    while (!skeletonCopy.isRoot(currentJoint)) {
        JointId parentId = skeletonCopy.parentId(currentJoint);

        skeleton.setParent(currentJoint, lastJoint);

        std::vector<JointId> children = skeletonCopy.children(currentJoint);
        children.push_back(parentId);

        lastJoint = currentJoint;
        currentJoint = parentId;
    }

    skeleton.setParent(currentJoint, lastJoint);

    skeleton.updateRoots();
}

}
