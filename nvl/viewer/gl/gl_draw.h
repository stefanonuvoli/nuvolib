/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_GL_DRAW_H
#define NVL_VIEWER_GL_DRAW_H

#include <nvl/nuvolib.h>

#ifdef NVL_OPENGL_LOADED

#include <nvl/math/point.h>
#include <nvl/utilities/color.h>

namespace nvl {

void glVertex(const Point3d& p);

void glTranslate(const Vector3d& t);
void glRotate(double angle, const Vector3d& axis);

void glNormal(const Vector3d& n);

void glColor(const Color& c);

}

#endif

#include "gl_draw.cpp"

#endif // NVL_VIEWER_GL_DRAW_H
