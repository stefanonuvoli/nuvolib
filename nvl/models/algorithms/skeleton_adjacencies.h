/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_SKELETON_ADJACENCIES_H
#define NVL_MODELS_SKELETON_ADJACENCIES_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Skeleton>
std::vector<std::vector<typename Skeleton::JointId>> skeletonJointJointAdjacencies(
        const Skeleton& skeleton,
        std::vector<std::vector<Index>>& childIds);


template<class Skeleton>
std::vector<typename Skeleton::JointId> skeletonJointDescendants(
        const Skeleton& skeleton,
        const typename Skeleton::JointId& jointId);

template<class Skeleton>
std::vector<typename Skeleton::JointId> skeletonJointNonDescendants(
        const Skeleton& skeleton,
        const typename Skeleton::JointId& jointId);

template<class Skeleton>
std::vector<unsigned int> skeletonJointDistance(
        const Skeleton& skeleton,
        const typename Skeleton::JointId& jointId);

template<class Skeleton>
std::vector<unsigned int> skeletonJointDistance(
        const Skeleton& skeleton,
        const std::vector<typename Skeleton::JointId>& seedJoints);

template<class Skeleton>
std::vector<std::vector<typename Skeleton::JointId>> skeletonRootLeafPaths(
        const Skeleton& skeleton);

namespace internal {

template<class Skeleton>
void skeletonJointDistanceRecursive(
        const Skeleton& skeleton,
        const typename Skeleton::JointId& jointId,
        std::vector<unsigned int>& distance,
        std::vector<bool>& computedJoints);

}

}

#include "skeleton_adjacencies.cpp"

#endif // NVL_MODELS_SKELETON_ADJACENCIES_H
