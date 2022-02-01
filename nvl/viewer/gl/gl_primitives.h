/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_GL_PRIMITIVES_H
#define NVL_VIEWER_GL_PRIMITIVES_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>

#include <nvl/utilities/color.h>

namespace nvl {

void drawPoint(
        const Point3d& point,
        const Color& color,
        int pointSize);

void drawLine(
        const Point3d& start,
        const Point3d& end,
        const Color& color,
        int lineSize);

void drawSphere(
        const Point3d& center,
        double radius,
        const Color& color,
        int slices,
        int stacks);

void drawCylinder(
        const Point3d& start,
        const Point3d& end,
        double top_radius,
        double bottom_radius,
        const Color &color,
        int slices,
        int stacks);

void drawArrow(
        const Point3d& start,
        const Point3d& end,
        double bottom_radius,
        const Color& color,
        int slices,
        int stacks);

}

#include "gl_primitives.cpp"

#endif // NVL_VIEWER_GL_PRIMITIVES_H
