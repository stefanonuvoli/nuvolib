/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_OCTREENODE_H
#define NVL_OCTREENODE_H

#include <array>
#include <vector>

namespace nvl {

namespace internal {

/**
 * @brief The node of the AABB tree
 *
 */
template<class K, class T, class R>
class OctreeNode {

public:

    typedef enum { X0Y0Z0 = 0, X0Y0Z1 = 1, X0Y1Z0 = 2, X0Y1Z1 = 3, X1Y0Z0 = 4, X1Y0Z1 = 5, X1Y1Z0 = 6, X1Y1Z1 = 7 } OctreeNodePosition;
    typedef enum { X0 = 0, X1 = 1, Y0 = 2, Y1 = 3, Z0 = 4, Z1 = 5 } OctreeDirection;


    /* Constructors/Destructor */

    OctreeNode(
        const K& center,
        const R& radius);


    /* Fields (public) */

    std::vector<std::pair<K,T>> values;

    K center;
    R radius;

    OctreeNode<K,T,R>* parent;
    std::array<OctreeNode<K,T,R>*, 8> children;

    OctreeNodePosition position;


    /* Public methods */

    void split();
    bool contains(const K& key) const;
    std::vector<const OctreeNode<K,T,R>*> neighbors(
            const OctreeDirection& direction) const;
    std::vector<const OctreeNode<K,T,R>*> neighbors() const;
    inline bool isLeaf() const;


private:

    /* Private methods */

    const OctreeNode<K,T,R>* getNeighborHelper(
            const OctreeDirection& direction) const;
    std::vector<const OctreeNode<K,T,R>*> getNeighborLeavesHelper(
            const OctreeDirection& direction,
            const OctreeNode<K,T,R>* neighbor) const;

    inline void init(
        const K& center,
        const R& radius);
};

}

}

#include "octree_node.cpp"

#endif // NVL_OCTREENODE_H
