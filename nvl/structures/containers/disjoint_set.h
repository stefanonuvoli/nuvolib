/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_STRUCTURES_DISJOINT_SET_H
#define NVL_STRUCTURES_DISJOINT_SET_H

#include <nvl/nuvolib.h>

#include <map>
#include <vector>

namespace nvl {

/**
 * @brief Disjoint set class. It allows to efficiently perform find-union operations
 * on disjoint sets.
 * @tparam T Type of the elements
 * @tparam C Map type to link the elements to the index
 */
template<class T, class C = std::map<T, Index>>
class DisjointSet
{

public:

    DisjointSet();
    DisjointSet(const std::vector<T>& elements);

    void insert(const std::vector<T>& elements);
    bool insert(const T& element);

    Index find(const T& id) const;

    bool merge(const T& a, const T& b);

    std::vector<std::vector<T>> computeSets();

    void clear();


protected:

    bool mergeHelper(const Index& a, const Index& b);
    Index findHelper(const Index& id) const;

    std::vector<Index> vParent;
    std::vector<T> vElements;
    C vMap;

};

}

#include "disjoint_set.cpp"

#endif // NVL_STRUCTURES_DISJOINT_SET_H
