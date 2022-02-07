/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_AVLHELPERS_H
#define NVL_AVLHELPERS_H

#include "tree_common.h"

#include <vector>

namespace nvl {

namespace internal {

/* AVL helpers */

template<class Node>
inline TreeSize getHeightHelper(const Node* node);

template<class Node>
inline void updateHeightHelper(Node* node);

template<class Node>
inline void rebalanceHelper(Node* node, Node*& rootNode);

template<class Node>
inline void updateHeightAndRebalanceHelper(Node* node, Node*& rootNode);


/* AVL Rotations helper */

template<class Node>
inline Node* leftRotateHelper(Node* a);

template<class Node>
inline Node* rightRotateHelper(Node* a);

}

}

#include "avl_helpers.cpp"

#endif // NVL_AVLHELPERS_H
