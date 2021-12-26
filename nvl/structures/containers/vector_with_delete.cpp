/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "vector_with_delete.h"

#include <nvl/utilities/vector_utils.h>

namespace nvl {

/**
 * @brief Default constructor
 */
template<class T>
VectorWithDelete<T>::VectorWithDelete()
{
    vNumberElements = 0;
}

/**
 * @brief Constructor with default size
 * @param size Size of the vector
 */
template<class T>
VectorWithDelete<T>::VectorWithDelete(const Size size)
{
    vVector.resize(size);
    vIsDeleted.resize(size, false);
    vNumberElements = size;
}

/**
 * @brief Constructor with default size
 * @param size Size of the vector
 * @param value Initialization value
 */
template<class T>
VectorWithDelete<T>::VectorWithDelete(const Size size, const T& value)
{
    vVector.resize(size, value);
    vIsDeleted.resize(size, false);
    vNumberElements = size;
}

/**
 * @brief Clear the vector
 */
template<class T>
void VectorWithDelete<T>::clear()
{
    vVector.clear();
    vIsDeleted.clear();
    vNumberElements = 0;
}

/**
 * @brief Get the real size of the vector (counting also the deleted elements)
 * @return Real size of the vector
 */
template<class T>
Size VectorWithDelete<T>::realSize() const
{
    return vVector.size();
}

/**
 * @brief Get the number of elements (not counting the deleted ones)
 * @return Number of elements in the vector
 */
template<class T>
Size VectorWithDelete<T>::size() const
{
    return vNumberElements;
}

/**
 * @brief Check if vector is empty
 * @return True if the vector is empty, else otherwise
 */
template<class T>
bool VectorWithDelete<T>::empty() const
{
    return size() == 0;
}

/**
 * @brief Resize the vector
 * @param size New size
 * @param initValue Initialization value
 */
template<class T>
void VectorWithDelete<T>::resize(const Size size, const T initValue)
{
    vVector.resize(size, initValue);
    vIsDeleted.resize(size, false);
    vNumberElements += size - vNumberElements;
}

/**
 * @brief Get element at a given position in the vector
 * @param pos Position
 * @return The element
 */
template<class T>
T& VectorWithDelete<T>::at(const Index pos)
{
    assert(pos < vVector.size() && "Index exceed the vector dimension.");
    assert(!vIsDeleted[pos] && "Accessing to a deleted element in vector with delete.");
    return vVector[pos];
}

/**
 * @brief Get element at a given position in the vector
 * @param pos Position
 * @return The element
 */
template<class T>
const T& VectorWithDelete<T>::at(const Index pos) const
{
    assert(pos < vVector.size() && "Index exceed the vector dimension.");
    assert(!vIsDeleted[pos] && "Accessing to a deleted element in vector with delete.");
    return vVector[pos];
}

/**
 * @brief Add an element to the back of the vector
 * @param value Value to insert
 * @return The index where the element has been inserted
 */
template<class T>
Index VectorWithDelete<T>::push_back(const T& value)
{
    vVector.push_back(value);
    vIsDeleted.push_back(false);
    ++vNumberElements;

    return vVector.size() - 1;
}

/**
 * @brief Insert element in a given position
 * @param pos Position
 * @param value Element
 */
template<class T>
void VectorWithDelete<T>::insert(const Index pos, const T& value)
{
    assert(pos <= vVector.size() && "Index exceed the vector dimension.");

    this->vVector.insert(this->vVector.begin() + pos, value);
    this->vIsDeleted.insert(this->vIsDeleted.begin() + pos, false);
}

/**
 * @brief Erase last element from vector
 */
template<class T>
void VectorWithDelete<T>::pop_back()
{
    erase(this->end()-1);
}

/**
 * @brief Erase an element from the vector, given the position
 * @param pos Position
 */
template<class T>
void VectorWithDelete<T>::erase(const Index pos)
{
    assert(pos < vVector.size() && "Index exceed the vector dimension.");
    assert(!vIsDeleted[pos] && "Accessing to a deleted element in vector with delete.");
    vIsDeleted[pos] = true;
    --vNumberElements;
}

/**
 * @brief Erase an element from the vector, given the iterator
 * @param it Iterator
 */
template<class T>
void VectorWithDelete<T>::erase(iterator it)
{
    erase(id(it));
}

/**
 * @brief Erase an element from the vector, given the iterator
 * @param it Iterator
 */
template<class T>
void VectorWithDelete<T>::erase(const_iterator it)
{
    erase(id(it));
}

/**
 * @brief Get first element in the vector
 * @return First element
 */
template<class T>
const T& VectorWithDelete<T>::front() const
{
    assert(vNumberElements > 0);
    return *(this->begin());
}

/**
 * @brief Get last element in the vector
 * @return Last element
 */
template<class T>
const T& VectorWithDelete<T>::back() const
{
    assert(vNumberElements > 0);
    return *(this->end()-1);
}

/**
 * @brief Check if an element at a given position has been deleted
 * @param pos Position
 * @return True if the element is deleted, false otherwise
 */
template<class T>
bool VectorWithDelete<T>::isDeleted(const Index pos) const
{
    return vIsDeleted[pos];
}

/**
 * @brief Compact the vector, erasing from memory the deleted elements
 * @return Map from the new index to the previous ones
 */
template<class T>
std::vector<Index> VectorWithDelete<T>::compact() {
    std::vector<Index> map = vectorRemoveElements(vVector, vIsDeleted);

    vIsDeleted.resize(vVector.size());
    std::fill(vIsDeleted.begin(), vIsDeleted.end(), false);

    return map;
}

/**
 * @brief Acces operation
 * @param id Position
 * @return Element in that position
 */
template<class T>
T& VectorWithDelete<T>::operator[](const Index id)
{
    return vVector[id];
}

/**
 * @brief Acces operation
 * @param id Position
 * @return Element in that position
 */
template<class T>
const T& VectorWithDelete<T>::operator[](const Index id) const
{
    return vVector[id];
}

/**
 * @brief Get begin iterator
 * @return Begin iterator
 */
template<class T>
typename VectorWithDelete<T>::iterator VectorWithDelete<T>::begin()
{
    Index i = 0;
    typename std::vector<T>::iterator it = vVector.begin();
    while (it != vVector.end() && vIsDeleted[i]) {
        ++it;
        ++i;
    }
    return VectorWithDelete<T>::iterator(this, it);
}

/**
 * @brief Get end iterator
 * @return End iterator
 */
template<class T>
typename VectorWithDelete<T>::iterator VectorWithDelete<T>::end()
{
    return VectorWithDelete<T>::iterator(this, vVector.end());
}

/**
 * @brief Get begin iterator
 * @return Begin iterator
 */
template<class T>
typename VectorWithDelete<T>::const_iterator VectorWithDelete<T>::begin() const
{
    Index pos = 0;
    typename std::vector<T>::const_iterator it = vVector.begin();
    while (it != vVector.end() && vIsDeleted[pos]) {
        ++it;
        ++pos;
    }
    return VectorWithDelete<T>::const_iterator(this, it);
}

/**
 * @brief Get end iterator
 * @return End iterator
 */
template<class T>
typename VectorWithDelete<T>::const_iterator VectorWithDelete<T>::end() const
{
    return VectorWithDelete<T>::const_iterator(this, vVector.end());
}

/**
 * @brief Get position given an iterator
 * @return Position
 */
template<class T>
Index VectorWithDelete<T>::id(iterator it)
{
    return std::distance(vVector.begin(), it.containerIterator());
}

/**
 * @brief Get position given an iterator
 * @return Position
 */
template<class T>
Index VectorWithDelete<T>::id(const_iterator it) const
{
    return std::distance(vVector.begin(), it.containerIterator());
}

/**
 * @brief Operator <<, for printing
 * @param output Stream output
 * @param vector Vector
 * @return Output stream reference
 */
template<class T>
std::ostream& operator<<(std::ostream& output, const VectorWithDelete<T>& vector)
{
    for (const T& value : vector) {
        output << value << " ";
    }

    return output;
}

}
