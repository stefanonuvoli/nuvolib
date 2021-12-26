/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_STRUCTURES_VECTOR_WITH_DELETE_ITERATOR_H
#define NVL_STRUCTURES_VECTOR_WITH_DELETE_ITERATOR_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

namespace internal {

/**
 * @brief Iterator for the vector with deleted
 * @tparam C Container used
 * @tparam T Type of the element
 * @tparam I Iterator of the container
 */
template<class C, class T, class I>
class VectorWithDeleteIterator
{

public:

    using difference_type = T;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::forward_iterator_tag;


    /* Constructors */

    VectorWithDeleteIterator();
    VectorWithDeleteIterator(
            C* vectorWithDelete,
            I it);

    /* Methods */

    I& containerIterator();
    const I& containerIterator() const;

    bool operator==(const VectorWithDeleteIterator& otherIterator) const;
    bool operator!=(const VectorWithDeleteIterator& otherIterator) const;

    VectorWithDeleteIterator operator++();
    VectorWithDeleteIterator operator++(int);
    VectorWithDeleteIterator operator+(int);
    VectorWithDeleteIterator operator+=(int);
    VectorWithDeleteIterator operator--();
    VectorWithDeleteIterator operator--(int);
    VectorWithDeleteIterator operator-(int);
    VectorWithDeleteIterator operator-=(int);

    T& operator*() const;
    T* operator->() const;

private:

    void next();
    void prev();

private:

    C* vVectorWithDelete;
    I vIt;
};

}

}

#include "vector_with_delete_iterator.cpp"

#include <nvl/structures/containers/vector_with_delete.h>

#endif // NVL_STRUCTURES_VECTOR_WITH_DELETE_ITERATOR_H
