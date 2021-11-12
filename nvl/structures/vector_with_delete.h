#ifndef NVL_STRUCTURES_VECTOR_WITH_DELETE_H
#define NVL_STRUCTURES_VECTOR_WITH_DELETE_H

#include <nvl/nuvolib.h>

#include <nvl/structures/internal/vector_with_delete_iterator.h>

#include <vector>
#include <ostream>

namespace nvl {

template<class T>
class VectorWithDelete
{

public:

    /* Typedefs */

    typedef internal::VectorWithDeleteIterator<VectorWithDelete<T>, T, typename std::vector<T>::iterator> iterator;
    typedef internal::VectorWithDeleteIterator<const VectorWithDelete<T>, const T, typename std::vector<T>::const_iterator> const_iterator;


    /* Constructors */

    VectorWithDelete();
    VectorWithDelete(Size realSize);
    VectorWithDelete(Size realSize, const T& value);


    /* Methods */

    void clear();
    Size realSize() const;
    Size size() const;
    bool empty() const;
    void resize(Size size, T initValue = T());

    T& at(Index pos);
    const T& at(Index pos) const;

    Index push_back(const T& value);
    void insert(Index pos, const T& value);

    void pop_back();
    void erase(Index pos);
    void erase(iterator it);
    void erase(const_iterator it);

    const T& front() const;
    const T& back() const;

    bool isDeleted(Index pos) const;

    std::vector<Index> compact();

    T& operator[](const Index& pos);
    const T& operator[](const Index& pos) const;


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
