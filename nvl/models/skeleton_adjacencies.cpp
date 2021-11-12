#include "skeleton_adjacencies.h"

#include <queue>

#include <nvl/math/numeric_limits.h>

namespace nvl {

template<class Skeleton>
std::vector<std::vector<typename Skeleton::JointId>> skeletonJointJointAdjacencies(
        const Skeleton& skeleton,
        std::vector<std::vector<Index>>& childIds)
{
    typedef typename Skeleton::Joint Joint;
    typedef typename Skeleton::JointId JointId;

    std::vector<std::vector<JointId>> jjAdj;
    jjAdj.resize(skeleton.jointNumber());
    childIds.resize(skeleton.jointNumber());

    for (const Joint& joint : skeleton.joints()) {
        Index childPos = 0;
        for (const JointId& childId : skeleton.children(joint)) {
            jjAdj[joint.id()].push_back(childId);
            jjAdj[childId].push_back(joint.id());

            childIds[joint.id()].push_back(childPos);
            childIds[childId].push_back(MAX_INDEX);

            ++childPos;
        }
    }

    return jjAdj;
}

template<class Skeleton>
std::vector<typename Skeleton::JointId> skeletonJointDescendants(
        const Skeleton& skeleton,
        const typename Skeleton::JointId& jointId)
{
    typedef typename Skeleton::JointId JointId;

    std::vector<JointId> result;

    std::queue<JointId> queue;
    for (JointId child : skeleton.children(jointId)) {
        queue.push(child);
    }

    while (!queue.empty()) {
        JointId currentJointId = queue.front();
        queue.pop();

        result.push_back(currentJointId);

        for (JointId child : skeleton.children(currentJointId)) {
            queue.push(child);
        }
    }

    return result;
}

template<class Skeleton>
std::vector<typename Skeleton::JointId> skeletonJointNonDescendants(
        const Skeleton& skeleton,
        const typename Skeleton::JointId& jointId)
{
    typedef typename Skeleton::JointId JointId;

    std::vector<JointId> result;

    std::queue<JointId> queue;
    for (JointId root : skeleton.roots()) {
        if (root != jointId) {
            queue.push(root);
        }
    }

    while (!queue.empty()) {
        JointId currentJointId = queue.front();
        queue.pop();

        result.push_back(currentJointId);

        for (JointId child : skeleton.children(currentJointId)) {
            if (child != jointId) {
                queue.push(child);
            }
        }
    }

    return result;
}

template<class Skeleton>
std::vector<unsigned int> skeletonJointDistance(
        const Skeleton& skeleton,
        const typename Skeleton::JointId& seedJoint)
{
    std::vector<bool> computedJoints(skeleton.jointNumber(), false);

    std::vector<unsigned int> distance(skeleton.jointNumber(), nvl::maxLimitValue<unsigned int>());
    distance[seedJoint] = 0;

    internal::skeletonJointDistanceRecursive(skeleton, seedJoint, distance, computedJoints);

    return distance;
}

template<class Skeleton>
std::vector<unsigned int> skeletonJointDistance(
        const Skeleton& skeleton,
        const std::vector<typename Skeleton::JointId>& seedJoints)
{
    typedef typename Skeleton::JointId JointId;

    std::vector<unsigned int> distance(skeleton.jointNumber(), nvl::maxLimitValue<unsigned int>());
    for (JointId seedJoint : seedJoints) {
        std::vector<unsigned int> currentDistance = skeletonJointDistance(skeleton, seedJoint);

        for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
            distance[jId] = std::min(distance[jId], currentDistance[jId]);
        }
    }

    return distance;
}

template<class Skeleton>
std::vector<std::vector<typename Skeleton::JointId>> skeletonRootLeafPaths(
        const Skeleton& skeleton)
{
    typedef typename Skeleton::JointId JointId;

    std::vector<std::vector<JointId>> paths;
    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        if (skeleton.isLeaf(jId)) {
            std::vector<JointId> path;

            JointId currentJId = jId;
            while (!skeleton.isRoot(currentJId)) {
                path.push_back(currentJId);
                currentJId = skeleton.parentId(currentJId);
            }
            path.push_back(currentJId);

            std::reverse(path.begin(), path.end());

            paths.push_back(path);
        }
    }

    return paths;
}

namespace internal {

template<class Skeleton>
void skeletonJointDistanceRecursive(
        const Skeleton& skeleton,
        const typename Skeleton::JointId& jointId,
        std::vector<unsigned int>& distance,
        std::vector<bool>& computedJoints)
{
    typedef typename Skeleton::JointId JointId;

    assert(!computedJoints[jointId]);

    computedJoints[jointId] = true;

    if (!skeleton.isRoot(jointId)) {
        const JointId& parentId = skeleton.parentId(jointId);

        if (!computedJoints[parentId]) {
            distance[parentId] = 1 + distance[jointId];
            skeletonJointDistanceRecursive(skeleton, parentId, distance, computedJoints);
        }
    }
    for (const JointId& childId : skeleton.children(jointId)) {
        if (!computedJoints[childId]) {
            distance[childId] = 1 + distance[jointId];
            skeletonJointDistanceRecursive(skeleton, childId, distance, computedJoints);
        }
    }
}

}

}
