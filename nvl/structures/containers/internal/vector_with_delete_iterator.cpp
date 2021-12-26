/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "vector_with_delete_iterator.h"

namespace nvl {

namespace internal {

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I>::VectorWithDeleteIterator() :
    vVectorWithDelete(nullptr)
{

}

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I>::VectorWithDeleteIterator(
        C* vectorWithDelete,
        I it) :
    vVectorWithDelete(vectorWithDelete),
    vIt(it)
{

}

template<class C, class T, class I>
I& VectorWithDeleteIterator<C,T,I>::containerIterator()
{
    return vIt;
}

template<class C, class T, class I>
const I& VectorWithDeleteIterator<C,T,I>::containerIterator() const
{
    return vIt;
}

template<class C, class T, class I>
bool VectorWithDeleteIterator<C,T,I>::operator==(const VectorWithDeleteIterator<C,T,I>& otherIterator) const
{
    return (vIt == otherIterator.vIt);
}

template<class C, class T, class I>
bool VectorWithDeleteIterator<C,T,I>::operator!=(const VectorWithDeleteIterator<C,T,I>& otherIterator) const
{
    return !(*this == otherIterator);
}

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I> VectorWithDeleteIterator<C,T,I>::operator++()
{
    next();
    return *this;
}

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I> VectorWithDeleteIterator<C,T,I>::operator++(int)
{
    VectorWithDeleteIterator<C,T,I> oldIt = *this;
    next();
    return oldIt;
}

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I> VectorWithDeleteIterator<C,T,I>::operator+(int n)
{
    VectorWithDeleteIterator<C,T,I> newIt = *this;
    for (int i = 0; i < n; ++i) {
        ++newIt;
    }
    return newIt;
}

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I> VectorWithDeleteIterator<C,T,I>::operator+=(int n)
{
    for (int i = 0; i < n; ++i) {
        next();
    }
    return *this;
}

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I> VectorWithDeleteIterator<C,T,I>::operator--()
{
    prev();
    return *this;
}

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I> VectorWithDeleteIterator<C,T,I>::operator--(int)
{
    VectorWithDeleteIterator<C,T,I> oldIt = *this;
    prev();
    return oldIt;
}

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I> VectorWithDeleteIterator<C,T,I>::operator-(int n)
{
    VectorWithDeleteIterator<C,T,I> newIt = *this;
    for (int i = 0; i < n; ++i) {
        newIt--;
    }
    return newIt;
}

template<class C, class T, class I>
VectorWithDeleteIterator<C,T,I> VectorWithDeleteIterator<C,T,I>::operator-=(int n)
{
    for (int i = 0; i < n; ++i) {
        prev();
    }
    return *this;
}


template<class C, class T, class I>
T& VectorWithDeleteIterator<C,T,I>::operator*() const
{
    return *vIt;
}

template<class C, class T, class I>
T* VectorWithDeleteIterator<C,T,I>::operator->() const
{
    return &(*vIt);
}

template<class C, class T, class I>
void VectorWithDeleteIterator<C,T,I>::next()
{
    do {
        ++vIt;
    } while (*this != vVectorWithDelete->end() && vVectorWithDelete->isDeleted(vVectorWithDelete->id(*this)));
}

template<class C, class T, class I>
void VectorWithDeleteIterator<C,T,I>::prev()
{
    do {
        --vIt;
    } while (vVectorWithDelete->isDeleted(vVectorWithDelete->id(*this)));
}

}

}
