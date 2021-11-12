#include "numeric_limits.h"

namespace nvl {

template<class T>
T maxLimitValue()
{
    return std::numeric_limits<T>::max();
}

template<class T>
T minLimitValue()
{
    return std::numeric_limits<T>::lowest();
}

}
