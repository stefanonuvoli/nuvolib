/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_STRUCTURES_VECTOR_WITH_DELETE_H
#define NVL_STRUCTURES_VECTOR_WITH_DELETE_H

#include <nvl/nuvolib.h>

#include "internal/vector_with_delete_iterator.h"

#include <vector>
#include <ostream>

namespace nvl {

/**
 * @brief A vector which implements a lazy remove operation efficiently.
 * @tparam T Type of the element
 */
template<class T>
class VectorWithDelete
{

public:

    /* Typedefs */

    typedef internal::VectorWithDeleteIterator<VectorWithDelete<T>, T, typename std::vector<T>::iterator> iterator;
    typedef internal::VectorWithDeleteIterator<const VectorWithDelete<T>, const T, typename std::vector<T>::const_iterator> const_iterator;


    /* Constructors */

    VectorWithDelete();
    VectorWithDelete(const Size realSize);
    VectorWithDelete(const Size realSize, const T& value);


    /* Methods */

    void clear();
    Size realSize() const;
    Size size() const;
    bool empty() const;
    void resize(const Size size, const T initValue = T());

    T& at(const Index pos);
    const T& at(const Index pos) const;

    Index push_back(const T& value);
    void insert(const Index pos, const T& value);

    void pop_back();
    void erase(const Index pos);
    void erase(iterator it);
    void erase(const_iterator it);

    const T& front() const;
    const T& back() const;

    bool isDeleted(const Index pos) const;

    std::vector<Index> compact();

    T& operator[](const Index pos);
    const T& operator[](const Index pos) const;


    /* Iterators */

    const_iterator begin() const;
    const_iterator end() const;
    iterator begin();
    iterator end();

    Index id(iterator it);
    Index id(const_iterator it) const;


private:

    Size vNumberElements;
    std::vector<bool> vIsDeleted;
    std::vector<T> vVector;

};

template<class T>
std::ostream& operator<<(std::ostream& output, const VectorWithDelete<T>& vector);

}

#include "vector_with_delete.cpp"

#endif // NVL_STRUCTURES_VECTOR_WITH_DELETE_H
