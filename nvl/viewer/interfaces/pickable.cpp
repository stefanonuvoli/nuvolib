#include "pickable.h"

#include <nvl/math/point.h>

namespace nvl {

NVL_INLINE Pickable::Pickable(bool pickingEnabled) :
    vPickable(pickingEnabled)
{

}

NVL_INLINE bool Pickable::isPickable() const
{
    return vPickable;
}

NVL_INLINE void Pickable::setPickable(bool pickable)
{
    vPickable = pickable;
}

}
