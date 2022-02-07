/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef nvlBST_H
#define nvlBST_H

#include <nvl/nuvolib.h>

/** Default BST implementations */
#include "avlleaf.h"

namespace nvl {

/*
 * AVLLeaf implementation is the default one. The BST is an auto-balancing
 * AVL tree. Values are saved only in the leaves.
 */
template<class K, class T = K, class C = DefaultComparatorType<K>>
using BST = typename nvl::AVLLeaf<K, T, C>;

}

#endif // nvlBST_H
