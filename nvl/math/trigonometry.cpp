#include "trigonometry.h"

#include <math.h>

namespace nvl {

template<class T, class R>
R sin(const T& value) {
    return std::sin(value);
}

template<class T, class R>
R cos(const T& value) {
    return std::cos(value);
}

template<class T, class R>
R asin(const T& value) {
    return std::asin(value);
}

template<class T, class R>
R acos(const T& value) {
    return std::acos(value);
}

}
