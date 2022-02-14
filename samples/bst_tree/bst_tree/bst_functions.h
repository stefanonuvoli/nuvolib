/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef BST_FUNCTIONS_H
#define BST_FUNCTIONS_H

#include <nvl/math/point.h>

#include <nvl/structures/trees/bst.h>

//#include <nvl/structures/trees/avlleaf.h>
//#include <nvl/structures/trees/avlinner.h>
//#include <nvl/structures/trees/bstinner.h>
//#include <nvl/structures/trees/bstleaf.h>

/* Choose the one you prefer, you can use each one in the same way! */

typedef nvl::BST<int> TreeInt; //Default BST (key only)

//    typedef nvl::AVLLeaf<int> TreeInt;
//    typedef nvl::BSTLeaf<int> TreeInt;
//    typedef nvl::AVLInner<int> TreeInt;
//    typedef nvl::BSTInner<int> TreeInt;


/* Choose the one you prefer, you can use each one in the same way! */

typedef nvl::BST<int, std::string> TreeIntString; //Default BST (key-value)

//    typedef nvl::AVLLeaf<int, std::string> TreeIntString;
//    typedef nvl::BSTLeaf<int, std::string> TreeIntString;
//    typedef nvl::AVLInner<int, std::string> TreeIntString;
//    typedef nvl::BSTInner<int, std::string> TreeIntString;

bool reverseComparator(const int& o1, const int& o2);

// Custom comparator for 2D points
struct CustomPointComparator {
    nvl::Point2d p;

    CustomPointComparator(nvl::Point2d p) {
		this->p = p;
	}

    bool operator()(const nvl::Point2d& o1, const nvl::Point2d& o2) {
		return p.dot(o1) < p.dot(o2);
	}
};

#endif // BST_FUNCTIONS_H
