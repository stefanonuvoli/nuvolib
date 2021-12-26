/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "disjoint_set.h"

namespace nvl {

/**
 * @brief Default constructor
 */
template<class T, class C>
DisjointSet<T,C>::DisjointSet() {

}

/**
 * @brief Constructor with elements
 * @param elements Elements for initialization
 */
template<class T, class C>
DisjointSet<T,C>::DisjointSet(const std::vector<T>& elements) {
    insert(elements);
}

/**
 * @brief Insert a collection of elements in disjoint sets
 * @param elements Collection of elements
 */
template<class T, class C>
void DisjointSet<T,C>::insert(const std::vector<T>& elements) {
    for (const T& element : elements) {
        insert(element);
    }
}

/**
 * @brief Insert a new element
 * @param element Element to be inserted
 * @return
 */
template<class T, class C>
bool DisjointSet<T,C>::insert(const T& element) {
    typename C::iterator it = vMap.find(element);
    if (it == vMap.end()) {
        Index id = vElements.size();

        vElements.push_back(element);
        vParent.push_back(id);

        vMap[element] = id;

        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Find the set in which an element is belonging
 * @param element Element to be found
 * @return Id of the set in which the element belongs. It returns NULL_ID
 * if element is not in the data structure
 */
template<class T, class C>
Index DisjointSet<T,C>::find(const T& element) const {
    typename C::const_iterator it = vMap.find(element);
    if (it != vMap.end()) {
        return findHelper(it->second);
    }
    else {
        return NULL_ID;
    }
}

/**
 * @brief Merge (union of) two elements in a single set
 * @param a First element
 * @param b Second element
 * @return True if it was possible, false otherwise.
 */
template<class T, class C>
bool DisjointSet<T,C>::merge(const T& a, const T& b) {
    typename C::iterator it1 = vMap.find(a);
    typename C::iterator it2 = vMap.find(b);

    if (it1 != vMap.end() && it2 != vMap.end()) {
        return mergeHelper(it1->second, it2->second);
    }
    else {
        return false;
    }
}

/**
 * @brief Obtain all the disjoint sets
 * @return Collection of disjoint sets
 */
template<class T, class C>
std::vector<std::vector<T>> DisjointSet<T,C>::computeSets()
{
    std::vector<std::vector<T>> sets;

    std::vector<bool> computed(vElements.size(), false);
    for (Index i = 0; i < vElements.size(); i++) {
        Index si = findHelper(i);

        if (computed[si])
            continue;

        std::vector<T> set;
        set.push_back(vElements[i]);

        for (Index j = i+1; j < vElements.size(); j++) {
            Index sj = findHelper(j);
            if (si == sj) {
                set.push_back(vElements[j]);
            }
        }

        computed[si] = true;

        sets.push_back(set);
    }

    return sets;
}

/**
 * @brief Clear the data structure
 */
template<class T, class C>
void DisjointSet<T,C>::clear()
{
    vMap.clear();
    vElements.clear();
    vParent.clear();
}

/**
 * @brief Helper to find the set of element
 * @param id Id of the set
 * @return The id of the set which contains all the elements
 */
template<class T, class C>
Index DisjointSet<T,C>::findHelper(const Index& id) const {
    if (id == NULL_ID)
        return NULL_ID;

    if (vParent[id] == id) {
        return id;
    }

    return findHelper(vParent[id]);
}

/**
 * @brief Helper to merge the set of elements
 * @param a First set
 * @param b Second set
 * @return True if the sets have been merged
 */
template<class T, class C>
bool DisjointSet<T,C>::mergeHelper(const Index& a, const Index& b) {
    if (a == NULL_ID || b == NULL_ID) {
        return false;
    }

    Index x = findHelper(a);
    Index y = findHelper(b);

    vParent[x] = y;

    return true;
}

}
