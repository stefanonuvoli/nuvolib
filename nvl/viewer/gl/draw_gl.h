#ifndef NVL_VIEWER_DRAW_GL_H
#define NVL_VIEWER_DRAW_GL_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>
#include <nvl/utilities/color.h>

namespace nvl {

void glVertex(const Point3d& p);

void glTranslate(const Vector3d& t);
void glRotate(double angle, const Vector3d& axis);

void glNormal(const Vector3d& n);

void glColor(const Color& c);

}

#include "draw_gl.cpp"

#endif // NVL_VIEWER_DRAW_GL_H
