/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
