/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "drawable.h"

#include <nvl/math/point.h>

namespace nvl {


NVL_INLINE Drawable::Drawable(bool visible) :
    vVisible(visible)
{

}

NVL_INLINE bool Drawable::isVisible() const
{
    return vVisible;
}

NVL_INLINE void Drawable::setVisible(bool visible)
{
    vVisible = visible;
}

}
