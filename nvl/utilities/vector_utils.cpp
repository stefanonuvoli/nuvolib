#include "vector_utils.h"

#include <algorithm>

namespace nvl {

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

template<class T>
std::vector<Index> vectorRemoveElements(
        std::vector<T>& vector,
        const std::vector<bool>& isDeleted)
{
    std::vector<Index> map(vector.size(), MAX_INDEX);

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

template<class T>
std::vector<Index> vectorRemoveElement(
        std::vector<T>& vector,
        Index index)
{
    std::vector<Index> map(vector.size(), MAX_INDEX);

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

template<class T>
Index vectorGetIndexByValue(
        const std::vector<T>& vector,
        const T& value)
{
    typename std::vector<T>::const_iterator itr = std::find(vector.cbegin(), vector.cend(), value);

    Index id = MAX_INDEX;
    if (itr != vector.cend()) {
        id = std::distance(vector.cbegin(), itr);
    }
    return id;
}

}
