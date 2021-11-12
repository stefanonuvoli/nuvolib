#ifndef NVL_VIEWER_ANIMABLE_H
#define NVL_VIEWER_ANIMABLE_H

#include <nvl/nuvolib.h>

namespace nvl {

class Animable
{

public:

    Animable(bool animable = true);
    virtual ~Animable();

    virtual void startAnimation() = 0;
    virtual void pauseAnimation() = 0;
    virtual void stopAnimation() = 0;
    virtual bool animate() = 0;
    virtual bool animationRunning() const = 0;

    virtual bool isAnimable() const;
    virtual void setAnimable(bool animable);

protected:

    bool vAnimable;

};

}

#include "animable.cpp"

#endif // NVL_VIEWER_ANIMABLE_H
