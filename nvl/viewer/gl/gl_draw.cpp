/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "gl_draw.h"

#ifdef NVL_OPENGL_LOADED

#include <nvl/viewer/gl/opengl_headers.h>

namespace nvl {

NVL_INLINE void glVertex(const Point3d& p)
{
    glVertex3d(p.x(), p.y(), p.z());
}

NVL_INLINE void glTranslate(const Vector3d& t)
{
    glTranslated(t.x(), t.y(), t.z());
}

NVL_INLINE void glRotate(double angle, const Vector3d& axis) {
    glRotated(angle, axis.x(), axis.y(), axis.z());
}

NVL_INLINE void glNormal(const Vector3d& n)
{
    glNormal3d(n.x(), n.y(), n.z());
}

NVL_INLINE void glColor(const Color& c)
{
    glColor4f(c.redF(), c.greenF(), c.blueF(), c.alphaF());
}

}

#endif
