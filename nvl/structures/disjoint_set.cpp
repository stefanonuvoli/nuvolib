#include "disjoint_set.h"

namespace nvl {

template<class T, class C>
DisjointSet<T,C>::DisjointSet() {

}

template<class T, class C>
DisjointSet<T,C>::DisjointSet(const std::vector<T>& elements) {
    insert(elements);
}

template<class T, class C>
void DisjointSet<T,C>::insert(const std::vector<T>& elements) {
    for (const T& element : elements) {
        insert(element);
    }
}

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

template<class T, class C>
Index DisjointSet<T,C>::find(const T& element) const {
    typename C::const_iterator it = vMap.find(element);
    if (it != vMap.end()) {
        return findHelper(it->second);
    }
    else {
        return MAX_INDEX;
    }
}


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

template<class T, class C>
void DisjointSet<T,C>::clear()
{
    vMap.clear();
    vElements.clear();
    vParent.clear();
}

template<class T, class C>
Index DisjointSet<T,C>::findHelper(const Index& id) const {
    if (id == nvl::MAX_INDEX)
        return nvl::MAX_INDEX;

    if (vParent[id] == id) {
        return id;
    }

    return findHelper(vParent[id]);
}
template<class T, class C>
bool DisjointSet<T,C>::mergeHelper(const Index& a, const Index& b) {
    if (a == nvl::MAX_INDEX || b == nvl::MAX_INDEX) {
        return false;
    }

    Index x = findHelper(a);
    Index y = findHelper(b);

    vParent[x] = y;

    return true;
}

}
