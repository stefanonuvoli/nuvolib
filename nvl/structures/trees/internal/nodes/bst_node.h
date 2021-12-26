/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_BSTNODE_H
#define NVL_BSTNODE_H

namespace nvl {

namespace internal {

/**
 * @brief The node of the binary search tree
 *
 */
template <class K, class T>
class BSTNode {

public:

    /* Constructors/Destructor */

    BSTNode(const K& key, const T& value);
    BSTNode(const K& key);

    ~BSTNode();


    /* Fields */

    K key;
    T* value;

    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;


    /* Public methods */

    inline bool isLeaf() const;


private:

    /* Private methods */

    inline void init(const K& key, T* value);

};

}

}

#include "bst_node.cpp"

#endif // NVL_BSTNODE_H
