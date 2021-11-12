#include "frameable.h"

#include <nvl/math/point.h>

namespace nvl {

NVL_INLINE Frameable::Frameable()
{
    resetFrame();
}

NVL_INLINE void Frameable::setFrame(const Affine3d& frame)
{
    vFrame = frame;
}

NVL_INLINE const Affine3d& Frameable::frame() const
{
    return vFrame;
}

NVL_INLINE Affine3d& Frameable::frame()
{
    return vFrame;
}

NVL_INLINE void Frameable::resetFrame()
{
    vFrame.setIdentity();
}

}
