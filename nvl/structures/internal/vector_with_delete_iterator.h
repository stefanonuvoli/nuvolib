#ifndef NVL_STRUCTURES_VECTOR_WITH_DELETE_ITERATOR_H
#define NVL_STRUCTURES_VECTOR_WITH_DELETE_ITERATOR_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

namespace internal {

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

#include <nvl/structures/vector_with_delete.h>

#endif // NVL_STRUCTURES_VECTOR_WITH_DELETE_ITERATOR_H
