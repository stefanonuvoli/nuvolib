/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_TREEREVERSEITERATOR_H
#define NVL_TREEREVERSEITERATOR_H

#include "tree_genericiterator.h"
#include <iterator>

namespace nvl {

/**
 * @brief The reverse iterator class for BST
 */
template<class B, class N, class T>
class TreeReverseIterator :
        public TreeGenericIterator<B,N>,
        public std::iterator<std::bidirectional_iterator_tag, T>
{

public:

    /* Constructors */

    inline TreeReverseIterator(B* bst, N* node) :
        TreeGenericIterator<B,N>(bst,node) {}


    /* Iterator operators */

    inline bool operator == (const TreeReverseIterator& otherIterator) const;
    inline bool operator != (const TreeReverseIterator& otherIterator) const;

    inline TreeReverseIterator operator ++ ();
    inline TreeReverseIterator operator ++ (int);
    inline TreeReverseIterator operator + (int);
    inline TreeReverseIterator operator += (int);

    inline TreeReverseIterator operator -- ();
    inline TreeReverseIterator operator -- (int);
    inline TreeReverseIterator operator - (int);
    inline TreeReverseIterator operator -= (int);

    inline T& operator *() const;

protected:

    /* Protected methods */

    inline void next();
    inline void prev();

};



}


#include "tree_reverseiterator.cpp"

#endif // NVL_TREEREVERSEITERATOR_H
