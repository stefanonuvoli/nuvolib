#include "inverse_map.h"

namespace nvl {

template<class T>
std::vector<T> inverseMap(const std::vector<T>& map, const long long int size)
{
    long long int maxVId = -1;

    if (size > 0) {
        maxVId = size - 1;
    }
    else {
        for (const T& value : map) {
            maxVId = std::max(maxVId, static_cast<long long int>(value));
        }
    }

    if (maxVId < 0)
        return std::vector<T>();

    std::vector<Index> inverse(static_cast<Size>(maxVId + 1), MAX_INDEX);
    for (Index i = 0; i < map.size(); ++i) {
        if (map[i] == MAX_INDEX || map[i] < 0)
            continue;

        inverse[map[i]] = i;
    }

    return inverse;
}

template<class T>
std::map<T,T> inverseMap(const std::map<T,T>& map)
{
    std::map<T,T> inverse;
    for (Index i = 0; i < map.size(); ++i) {
        if (map[i] == MAX_INDEX || map[i] < 0)
            continue;

        inverse[map[i]] = i;
    }

    return inverse;
}

template<class T>
std::unordered_map<T,T> inverseMap(const std::unordered_map<T,T>& map)
{
    std::map<T,T> inverse;
    for (Index i = 0; i < map.size(); ++i) {
        if (map[i] == MAX_INDEX || map[i] < 0)
            continue;

        inverse[map[i]] = i;
    }

    return inverse;
}

}
