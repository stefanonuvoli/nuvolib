/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_TREEINSERTITERATOR_H
#define NVL_TREEINSERTITERATOR_H


#include <iterator>

namespace nvl {

/**
 * @brief The insert iterator class for BST
 */
template<class B, class K>
class TreeInsertIterator :
        public std::iterator<std::output_iterator_tag,void,void,void,void>  {
public:

    /* Constructors */

    inline TreeInsertIterator(B* bst) : bst(bst) {}


    /* Iterator operators */

    inline TreeInsertIterator& operator *();
    inline TreeInsertIterator& operator ++();
    inline TreeInsertIterator& operator ++(int);

    inline TreeInsertIterator& operator=(const K key);

private:

    /* Fields */

    B* bst;
};



}


#include "tree_insertiterator.cpp"

#endif // NVL_TREEINSERTITERATOR_H
