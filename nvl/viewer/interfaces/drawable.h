/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_DRAWABLE_H
#define NVL_VIEWER_DRAWABLE_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>

namespace nvl {

class Canvas;

class Drawable
{

public:

    Drawable(bool visible = true);
    virtual ~Drawable() = default;

    virtual void draw() const = 0;

    virtual Point3d sceneCenter() const = 0;
    virtual double sceneRadius() const = 0;

    virtual bool isVisible() const;
    virtual void setVisible(bool visible);


protected:

    bool vVisible;

};

}

#include "drawable.cpp"

#include <nvl/viewer/widgets/canvas.h>

#endif // NVL_VIEWER_DRAWABLE_H
