/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_PICKABLE_H
#define NVL_VIEWER_PICKABLE_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>

#include <vector>

namespace nvl {

class Canvas;

class Pickable
{

public:

    Pickable(bool pickingEnabled = true);
    virtual ~Pickable() = default;

    virtual void drawWithNames(Canvas* canvas, const Index drawableId) const = 0;

    virtual bool isPickable() const;
    virtual void setPickable(bool pickable);


protected:

    bool vPickable;

};

}

#include "pickable.cpp"

#include <nvl/viewer/widgets/canvas.h>

#endif // NVL_VIEWER_PICKABLE_H
