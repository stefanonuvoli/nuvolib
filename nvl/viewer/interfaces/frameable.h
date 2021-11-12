#ifndef NVL_VIEWER_FRAMEABLE_H
#define NVL_VIEWER_FRAMEABLE_H

#include <nvl/nuvolib.h>

#include <nvl/math/affine.h>

namespace nvl {

class Frameable
{

public:

    Frameable();
    virtual ~Frameable() = default;

    virtual void loadFrame() const = 0;
    virtual void unloadFrame() const = 0;

    void setFrame(const Affine3d& frame);
    const Affine3d& frame() const;
    Affine3d& frame();

    void resetFrame();


protected:

    Affine3d vFrame;

};

}

#include "frameable.cpp"

#endif // NVL_VIEWER_FRAMEABLE_H
