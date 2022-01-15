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

    while (currentJoint != NULL_ID) {
        skeleton.setParent(currentJoint, lastJoint);

        if (lastJoint != NULL_ID) {
            std::vector<JointId> children = skeletonCopy.children(lastJoint);
            children.push_back(currentJoint);
            skeleton.setChildren(lastJoint, children);
        }

        lastJoint = currentJoint;
        currentJoint = skeletonCopy.parentId(currentJoint);
    }

    skeleton.updateRoots();
}

}
