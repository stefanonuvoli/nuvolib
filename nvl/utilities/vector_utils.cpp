/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "vector_utils.h"

#include <algorithm>

namespace nvl {

/**
 * @brief Remove element from vector
 * @param vector Input vector
 * @param indexes Indexes to delete
 * @return Map of the indices of the new vector to the previous vector
 */
template<class T>
std::vector<Index> vectorRemoveElements(
        std::vector<T>& vector,
        const std::vector<Index>& indexes)
{
    std::vector<bool> isDeleted(vector.size(), false);
    for (const Index& id : indexes) {
        isDeleted[id] = true;
    }

    return vectorRemoveElements(vector, isDeleted);
}


/**
 * @brief Remove element from vector
 * @param vector Input vector
 * @param isDeleted Vector of flags which determine the elements to delete
 * @return Map of the indices of the new vector to the previous vector
 */
template<class T>
std::vector<Index> vectorRemoveElements(
        std::vector<T>& vector,
        const std::vector<bool>& isDeleted)
{
    std::vector<Index> map(vector.size(), NULL_ID);

    Size back = 0;

    for (Size i = 0; i < vector.size(); ++i) {
        if (isDeleted[i]) {
            ++back;
        }
        else {
            Size newId = i - back;

            vector[newId] = vector[i];

            map[i] = newId;
        }
    }

    vector.resize(vector.size() - back);

    return map;
}

/**
 * @brief Remove element from vector
 * @param vector Input vector
 * @param Index Index to delete
 * @return Map of the indices of the new vector to the previous vector
 */
template<class T>
std::vector<Index> vectorRemoveElement(
        std::vector<T>& vector,
        Index index)
{
    std::vector<Index> map(vector.size(), NULL_ID);

    for (Size i = 0; i < vector.size(); ++i) {
        if (i > index) {
            Size newId = i - 1;

            vector[newId] = vector[i];

            map[i] = newId;
        }
        else {
            map[i] = i;
        }
    }

    vector.resize(vector.size() - 1);

    return map;
}

/**
 * @brief Get index of an element in a vector
 * @param vector Input vector
 * @param value Input value
 * @return The index of the element if it has been found, NULL_ID otherwise
 */
template<class T>
Index vectorGetIndexByValue(
        const std::vector<T>& vector,
        const T& value)
{
    typename std::vector<T>::const_iterator itr = std::find(vector.cbegin(), vector.cend(), value);

    Index id = NULL_ID;
    if (itr != vector.cend()) {
        id = std::distance(vector.cbegin(), itr);
    }
    return id;
}

}
