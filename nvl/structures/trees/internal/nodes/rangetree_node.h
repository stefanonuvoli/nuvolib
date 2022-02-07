/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_RANGETREENODE_H
#define NVL_RANGETREENODE_H



namespace nvl {


template<class K, class T, class C>
class RangeTree;


namespace internal {

/**
 * @brief The node of the binary search tree
 *
 */
template<class K, class T, class C>
class RangeTreeNode {

public:

    /* Constructors/Destructor */

    RangeTreeNode(const K& key, const T& value);
    RangeTreeNode(const K& key);

    ~RangeTreeNode();


    /* Fields */

    K key;
    T* value;

    RangeTree<K,T,C>* assRangeTree;

    RangeTreeNode* parent;
    RangeTreeNode* left;
    RangeTreeNode* right;

    size_t height;


    /* Public methods */

    inline bool isLeaf() const;


private:

     /* Private methods */

    void init(const K& key, T* value);

};

}

}

#include "../../rangetree.h"

#include "rangetree_node.cpp"

#endif // NVL_RANGETREENODE_H
