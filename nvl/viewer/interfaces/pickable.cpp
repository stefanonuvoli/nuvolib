/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
