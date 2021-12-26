/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_BSTINNERHELPERS_H
#define NVL_BSTINNERHELPERS_H

#include "tree_common.h"

#include "bst_helpers.h"

#include <vector>

namespace nvl {

namespace internal {


/* Basic BST operation helpers */

template <class Node, class K, class C>
inline Node* insertNodeHelperInner(Node*& newNode, Node*& rootNode, C& comparator);

template <class Node>
inline Node* eraseNodeHelperInner(Node*& node, Node*& rootNode);

template <class Node, class K, class C>
inline Node* findNodeHelperInner(const K& key, Node*& rootNode, C& comparator);

template <class Node, class K, class C>
inline Node* findLowerHelperInner(const K& key, Node*& rootNode, C& comparator);

template <class Node, class K, class C>
inline Node* findUpperHelperInner(const K& key, Node*& rootNode, C& comparator);



/* Construction helpers */

template <class Node, class K, class C>
inline TreeSize constructionMedianHelperInner(
        std::vector<Node*>& sortedNodes,
        const TreeSize start, const TreeSize end,
        Node*& rootNode,
        C& comparator);



/* Range query helpers */

template <class Node, class K, class C>
inline Node* findSplitNodeHelperInner(
        const K& start, const K& end,
        Node* rootNode,
        C& comparator);

template <class Node>
inline void reportSubTreeHelperInner(
        Node* node,
        std::vector<Node*>& out);

template <class Node, class K, class C>
inline void rangeQueryHelperInner(
        const K& start, const K& end,
        std::vector<Node*> &out,
        Node* rootNode,
        C& comparator);



/* Successors/Predecessors Min/Max */

template <class Node>
inline Node* getSuccessorHelperInner(Node* node);

template <class Node>
inline Node* getPredecessorHelperInner(Node* node);

template <class Node>
inline Node* getMinimumHelperInner(Node* node);

template <class Node>
inline Node* getMaximumHelperInner(Node* node);

}

}

#include "bstinner_helpers.cpp"

#endif // NVL_BSTINNERHELPERS_H
