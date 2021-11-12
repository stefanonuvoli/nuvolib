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
