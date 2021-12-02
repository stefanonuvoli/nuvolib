#ifndef NVL_UTILITIES_VECTOR_UTILS_H
#define NVL_UTILITIES_VECTOR_UTILS_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class T>
std::vector<Index> vectorRemoveElements(
        std::vector<T>& vector,
        const std::vector<Index>& indexes);

template<class T>
std::vector<Index> vectorRemoveElements(
        std::vector<T>& vector,
        const std::vector<bool>& isDeleted);

template<class T>
std::vector<Index> vectorRemoveElement(
        std::vector<T>& vector,
        Index index);

template<class T>
Index vectorGetIndexByValue(
        const std::vector<T>& vector,
        const T& value);

}

#include "vector_utils.cpp"

#endif // NVL_UTILITIES_VECTOR_UTILS_H