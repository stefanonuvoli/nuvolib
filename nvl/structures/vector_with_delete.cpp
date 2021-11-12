#include "vector_with_delete.h"

#include <nvl/utilities/vector_utils.h>

namespace nvl {

template<class T>
VectorWithDelete<T>::VectorWithDelete()
{
    vNumberElements = 0;
}

template<class T>
VectorWithDelete<T>::VectorWithDelete(Size size)
{
    vVector.resize(size);
    vIsDeleted.resize(size, false);
    vNumberElements = size;
}

template<class T>
VectorWithDelete<T>::VectorWithDelete(Size size, const T& value)
{
    vVector.resize(size, value);
    vIsDeleted.resize(size, false);
    vNumberElements = size;
}

template<class T>
void VectorWithDelete<T>::clear()
{
    vVector.clear();
    vIsDeleted.clear();
    vNumberElements = 0;
}

template<class T>
Size VectorWithDelete<T>::realSize() const
{
    return vVector.size();
}

template<class T>
Size VectorWithDelete<T>::size() const
{
    return vNumberElements;
}

template<class T>
bool VectorWithDelete<T>::empty() const
{
    return size() == 0;
}

template<class T>
void VectorWithDelete<T>::resize(Size size, T initValue)
{
    vVector.resize(size, initValue);
}

template<class T>
T& VectorWithDelete<T>::at(Index pos)
{
    assert(pos < vVector.size() && "Index exceed the vector dimension.");
    assert(!vIsDeleted[pos] && "Accessing to a deleted element in vector with delete.");
    return vVector[pos];
}

template<class T>
const T& VectorWithDelete<T>::at(Index pos) const
{
    assert(pos < vVector.size() && "Index exceed the vector dimension.");
    assert(!vIsDeleted[pos] && "Accessing to a deleted element in vector with delete.");
    return vVector[pos];
}

template<class T>
Index VectorWithDelete<T>::push_back(const T& value)
{
    vVector.push_back(value);
    vIsDeleted.push_back(false);
    ++vNumberElements;

    return vVector.size() - 1;
}

template<class T>
void VectorWithDelete<T>::insert(Index pos, const T& value)
{
    assert(pos <= vVector.size() && "Index exceed the vector dimension.");

    this->vVector.insert(this->vVector.begin() + pos, value);
    this->vIsDeleted.insert(this->vIsDeleted.begin() + pos, false);
}

template<class T>
void VectorWithDelete<T>::pop_back()
{
    erase(this->end()-1);
}

template<class T>
void VectorWithDelete<T>::erase(Index pos)
{
    assert(pos < vVector.size() && "Index exceed the vector dimension.");
    assert(!vIsDeleted[pos] && "Accessing to a deleted element in vector with delete.");
    vIsDeleted[pos] = true;
    vNumberElements--;
}

template<class T>
void VectorWithDelete<T>::erase(iterator it)
{
    erase(id(it));
}

template<class T>
void VectorWithDelete<T>::erase(const_iterator it)
{
    erase(id(it));
}

template<class T>
const T& VectorWithDelete<T>::front() const
{
    assert(vNumberElements > 0);
    return *(this->begin());
}

template<class T>
const T& VectorWithDelete<T>::back() const
{
    assert(vNumberElements > 0);
    return *(this->end()-1);
}

template<class T>
bool VectorWithDelete<T>::isDeleted(Index pos) const
{
    return vIsDeleted[pos];
}

template<class T>
std::vector<Index> VectorWithDelete<T>::compact() {
    std::vector<Index> map = nvl::vectorRemoveElements(vVector, vIsDeleted);

    vIsDeleted.resize(vVector.size());
    std::fill(vIsDeleted.begin(), vIsDeleted.end(), false);

    return map;
}

template<class T>
T& VectorWithDelete<T>::operator[](const Size &id)
{
    return vVector[id];
}

template<class T>
const T& VectorWithDelete<T>::operator[](const Size &id) const
{
    return vVector[id];
}

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

template<class T>
typename VectorWithDelete<T>::iterator VectorWithDelete<T>::end()
{
    return VectorWithDelete<T>::iterator(this, vVector.end());
}

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

template<class T>
typename VectorWithDelete<T>::const_iterator VectorWithDelete<T>::end() const
{
    return VectorWithDelete<T>::const_iterator(this, vVector.end());
}

template<class T>
Index VectorWithDelete<T>::id(iterator it)
{
    return std::distance(vVector.begin(), it.containerIterator());
}

template<class T>
Index VectorWithDelete<T>::id(const_iterator it) const
{
    return std::distance(vVector.begin(), it.containerIterator());
}

template<class T>
std::ostream& operator<<(std::ostream& output, const VectorWithDelete<T>& vector)
{
    for (const T& value : vector) {
        output << value << " ";
    }

    return output;
}

}
