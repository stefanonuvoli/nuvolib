#ifndef NVL_MATH_INVERSE_MAP_H
#define NVL_MATH_INVERSE_MAP_H

#include <nvl/nuvolib.h>

#include <vector>
#include <map>
#include <unordered_map>

namespace nvl {

template<class T>
std::vector<T> inverseMap(const std::vector<T>& map, const long long int size = -1);

template<class Map, class T>
std::map<T,T> inverseMap(const std::map<T,T>& map);

template<class Map, class T>
std::unordered_map<T,T> inverseMap(const std::unordered_map<T,T>& map);

}


#include "inverse_map.cpp"

#endif // NVL_MATH_INVERSE_MAP_H
