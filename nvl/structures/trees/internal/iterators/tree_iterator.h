/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_TREEITERATOR_H
#define NVL_TREEITERATOR_H

#include "tree_genericiterator.h"
#include <iterator>

namespace nvl {


/**
 * @brief The iterator class for BST
 */
template<class B, class N, class T>
class TreeIterator :
        public TreeGenericIterator<B,N>,
        public std::iterator<std::bidirectional_iterator_tag, T>
{

public:

    /* Constructors */

    inline TreeIterator(B* bst, N* node) :
        TreeGenericIterator<B,N>(bst, node) {}


    /* Iterator operators */

    inline bool operator == (const TreeIterator& otherIterator) const;
    inline bool operator != (const TreeIterator& otherIterator) const;

    inline TreeIterator operator ++ ();
    inline TreeIterator operator ++ (int);
    inline TreeIterator operator + (int);
    inline TreeIterator operator += (int);

    inline TreeIterator operator -- ();
    inline TreeIterator operator -- (int);
    inline TreeIterator operator - (int);
    inline TreeIterator operator -= (int);

    inline T& operator *() const;


protected:

    /* Protected methods */

    inline void next();
    inline void prev();

};


}


#include "tree_iterator.cpp"

#endif // NVL_TREEITERATOR_H
