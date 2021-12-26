/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "tree_insertiterator.h"


namespace nvl {


/* --------- ITERATOR OPERATOR OVERLOAD --------- */

template <class B, class K>
TreeInsertIterator<B,K>& TreeInsertIterator<B,K>::operator=(const K key)
{
    this->bst->insert(key);

    return *this;
}

template <class B, class K>
TreeInsertIterator<B,K>& TreeInsertIterator<B,K>::operator*() {
    return *this;
}
template <class B, class K>
TreeInsertIterator<B,K>& TreeInsertIterator<B,K>::operator++() {
    return *this;
}
template <class B, class K>
TreeInsertIterator<B,K>& TreeInsertIterator<B,K>::operator++(int) {
    return *this;
}



}
