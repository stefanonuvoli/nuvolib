#include "comparators.h"

namespace nvl {

template<class T, typename... Ts>
bool pairwiseLessComparator(const T& v1, const T& v2, Ts... values)
{
    if (v1 < v2) {
        return true;
    }
    else if (v1 > v2) {
        return false;
    }
    else {
        return pairwiseLessComparator(values...);
    }
}

NVL_INLINE bool pairwiseLessComparator()
{
    return false;
}

}
