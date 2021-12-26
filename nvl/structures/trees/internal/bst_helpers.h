/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_BSTHELPERS_H
#define NVL_BSTHELPERS_H

#include "tree_common.h"

#include <vector>

namespace nvl {

namespace internal {

    /* Basic BST operation helpers */

    template <class Node>
    inline void clearHelper(Node*& rootNode);

    template <class Node, class T>
    inline Node* copySubtreeHelper(
            const Node* rootNode,
            Node* parent = nullptr);


    /* Utilities */

    template <class Node>
    inline TreeSize getHeightRecursiveHelper(const Node* node);

    template <class Node>
    inline void replaceSubtreeHelperInner(
            const Node* u,
            Node* v,
            Node*& rootNode);

}

}

#include "bst_helpers.cpp"

#endif // NVL_BSTHELPERS_H
