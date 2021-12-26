#include "gl_frameable.h"

#ifdef NVL_OPENGL_LOADED

namespace nvl {

NVL_INLINE GLFrameable::GLFrameable() : Frameable()
{

}

NVL_INLINE void GLFrameable::loadFrame() const
{
    glPushMatrix();
    glMultMatrixd(this->vFrame.matrix().data());
}

NVL_INLINE void GLFrameable::unloadFrame() const
{
    glPopMatrix();
}

}

#endif
