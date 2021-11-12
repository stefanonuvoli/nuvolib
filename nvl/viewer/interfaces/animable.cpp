#include "animable.h"

#include <nvl/math/point.h>

namespace nvl {

NVL_INLINE Animable::Animable(bool animable) :
    vAnimable(animable)
{

}

NVL_INLINE Animable::~Animable()
{

}

NVL_INLINE bool Animable::isAnimable() const
{
    return vAnimable;
}

NVL_INLINE void Animable::setAnimable(bool animable)
{
    vAnimable = animable;
}


}
