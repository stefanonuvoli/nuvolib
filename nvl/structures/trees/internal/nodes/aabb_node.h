/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_AABBNODE_H
#define NVL_AABBNODE_H

#include "../tree_common.h"

#include <array>

namespace nvl {

namespace internal {

/**
 * @brief The node of the AABB tree
 *
 */
template<int D, class K, class T>
class AABBNode {

public:

    /**
     * @brief D-dimensional axis-aligned bounding box
     */
    struct AABB {
        std::array<double, D> min;
        std::array<double, D> max;

        AABB() {
            min.fill(0.0f);
            max.fill(0.0f);
        }
    };

    /* Constructors/Destructor */

    AABBNode(const K& key, const T& value);
    AABBNode(const K& key);

    ~AABBNode();


    /* Fields */

    K key;
    T* value;

    AABB aabb;

    AABBNode* parent;
    AABBNode* left;
    AABBNode* right;

    TreeSize height;


    /* Public methods */

    inline bool isLeaf() const;


private:

    /* Private methods */

    inline void init(const K& key, T* value);
};

}

}

#include "aabb_node.cpp"

#endif // NVL_AABBNODE_H
