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
