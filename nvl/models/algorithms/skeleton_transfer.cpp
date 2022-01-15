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

    std::vector<JointId> jointMap(skeleton.jointNumber(), NULL_ID);

    std::unordered_set<JointId> jointSet(joints.begin(), joints.end());

    //Recursively add childs
    bool done;
    do {
        done = true;

        for (JointId i = 0; i < joints.size(); ++i) {
            JointId jId = joints[i];

            if (jointMap[jId] != NULL_ID)
                continue;

            done = false;

            assert(jointSet.find(jId) != jointSet.end());

            const Joint& joint = skeleton.joint(jId);
            JointId parent = skeleton.parentId(jId);

            JointId newJId = NULL_ID;
            if (skeleton.isRoot(jId) || jointSet.find(parent) == jointSet.end()) {
                if (targetParentId == NULL_ID) {
                    newJId = skeleton.addRoot(joint);
                }
                else {
                    newJId = skeleton.addChild(joint, targetParentId);
                }
            }
            else if (jointMap[parent] != NULL_ID) {
                const Joint& joint = skeleton.joint(jId);
                newJId = targetSkeleton.addChild(joint, jointMap[parent]);
            }

            if (newJId != NULL_ID) {
                jointMap[jId] = newJId;
                assert(birthJoint.size() == newJId);
                birthJoint.push_back(jId);
            }
        }
    }
    while (!done);
}


}
