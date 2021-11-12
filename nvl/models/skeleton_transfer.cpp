#include "skeleton_transfer.h"

#include <unordered_set>
namespace nvl {


/**
 * @brief Append joints to a skeleton
 * @param skeleton Input skeleton
 * @param joints Id of the joints to be appended
 * @param targetSkeleton Target skeleton
 * @param birthJoint Birth input skeleton joint
 * @param targetParent Target skeleton joint in which the input joints will be appended
 */
template<class Skeleton>
void skeletonTransferJoints(
        const Skeleton& skeleton,
        const std::vector<typename Skeleton::JointId>& joints,
        Skeleton& targetSkeleton,
        std::vector<typename Skeleton::JointId>& birthJoint,
        typename Skeleton::JointId targetParentId)
{
    //TODO TEST!
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Joint Joint;

    birthJoint.clear();

    std::vector<JointId> jointMap(skeleton.jointNumber(), MAX_INDEX);

    std::unordered_set<JointId> jointSet(joints.begin(), joints.end());

    //Recursively add childs
    bool done;
    do {
        done = true;

        for (JointId i = 0; i < joints.size(); ++i) {
            JointId jId = joints[i];

            if (jointMap[jId] != MAX_INDEX)
                continue;

            done = false;

            assert(jointSet.find(jId) != jointSet.end());

            const Joint& joint = skeleton.joint(jId);
            JointId parent = skeleton.parentId(jId);

            JointId newJId = MAX_INDEX;
            if (skeleton.isRoot(jId) || jointSet.find(parent) == jointSet.end()) {
                if (targetParentId == MAX_INDEX) {
                    newJId = skeleton.addRoot(joint);
                }
                else {
                    newJId = skeleton.addChild(targetParentId, joint);
                }
            }
            else if (jointMap[parent] != MAX_INDEX) {
                const Joint& joint = skeleton.joint(jId);
                newJId = targetSkeleton.addChild(jointMap[parent], joint);
            }

            if (newJId != MAX_INDEX) {
                jointMap[jId] = newJId;
                assert(birthJoint.size() == newJId);
                birthJoint.push_back(jId);
            }
        }
    }
    while (!done);
}


}
