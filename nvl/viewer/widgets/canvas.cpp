/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "canvas.h"

#ifdef NVL_QT

#include <nvl/utilities/vector_utils.h>

namespace nvl {

NVL_INLINE Canvas::Canvas() :
    vMovableFrame(Affine3d::Identity()),
    vAnimationsRunning(false),
    vAnimationsPaused(false)
{

}

NVL_INLINE Size Canvas::drawableNumber() const
{
    return vDrawables.size();
}

NVL_INLINE Drawable* Canvas::drawable(const Index& id) const
{
    return vDrawables[id];
}

NVL_INLINE const std::vector<Drawable*>& Canvas::drawables() const
{
    return vDrawables;
}

NVL_INLINE Index Canvas::addDrawable(Drawable* drawable, const std::string& name, const bool listable)
{
    const Index& id = vDrawables.size();

    vDrawables.push_back(drawable);
    vNames.push_back(name);

    Pickable* pickable = dynamic_cast<Pickable*>(drawable);
    vPickables.push_back(pickable);

    Animable* animable = dynamic_cast<Animable*>(drawable);
    vAnimables.push_back(animable);

    Frameable* frameable = dynamic_cast<Frameable*>(drawable);
    vFrameables.push_back(frameable);

    vListable.push_back(listable);

    return id;
}

NVL_INLINE bool Canvas::removeDrawable(const Index& id)
{
    if (id >= vDrawables.size())
        return false;

    std::vector<Index> map = vectorRemoveElement(vDrawables, id);
    vectorRemoveElement(vAnimables, id);
    vectorRemoveElement(vPickables, id);
    vectorRemoveElement(vFrameables, id);
    vectorRemoveElement(vNames, id);
    vectorRemoveElement(vListable, id);

    return true;
}

NVL_INLINE bool Canvas::removeDrawable(Drawable* drawable)
{
    std::vector<Drawable*>::iterator it = std::find(vDrawables.begin(), vDrawables.end(), drawable);
    if (it != vDrawables.end()) {
        return removeDrawable(std::distance(vDrawables.begin(), it));
    }
    else {
        return false;
    }
}

NVL_INLINE const std::string& Canvas::drawableName(const Index& id) const
{
    return vNames[id];
}

NVL_INLINE void Canvas::setDrawableName(const Index& id, const std::string& name)
{
    vNames[id] = name;
}

NVL_INLINE Animable* Canvas::animable(const Index& id)
{
    return vAnimables[id];
}

NVL_INLINE Pickable* Canvas::pickable(const Index& id)
{
    return vPickables[id];
}

NVL_INLINE Frameable *Canvas::frameable(const Index& id)
{
    return vFrameables[id];
}

NVL_INLINE bool Canvas::isAnimable(const Index& id) const
{
    return vAnimables[id] != nullptr;
}

NVL_INLINE bool Canvas::isPickable(const Index& id) const
{
    return vPickables[id] != nullptr;
}

NVL_INLINE bool Canvas::isFrameable(const Index& id) const
{
    return vFrameables[id] != nullptr;
}

NVL_INLINE bool Canvas::isListable(const Index &id) const
{
    return vListable[id];
}

NVL_INLINE void Canvas::setListable(const Index &id, const bool listable)
{
    vListable[id] = listable;
}

NVL_INLINE const Affine3d& Canvas::movableFrame() const
{
    return vMovableFrame;
}

NVL_INLINE void Canvas::setMovableFrame(const Affine3d& frame)
{
    vMovableFrame = frame;
}

NVL_INLINE void Canvas::startAnimations()
{
    #pragma omp parallel for
    for (Index id = 0; id < vDrawables.size(); ++id) {
        if (isAnimable(id)) {
            vAnimables[id]->startAnimation();
        }
    }

    vAnimationsPaused = false;
    vAnimationsRunning = true;

    this->updateGL();
}

NVL_INLINE void Canvas::pauseAnimations()
{
    if (vAnimationsRunning) {
        #pragma omp parallel for
        for (Index id = 0; id < vDrawables.size(); ++id) {
            if (isAnimable(id)) {
                vAnimables[id]->pauseAnimation();
            }
        }

        vAnimationsPaused = true;

        this->updateGL();
    }
}
NVL_INLINE void Canvas::stopAnimations()
{
    if (vAnimationsRunning || vAnimationsPaused) {
        #pragma omp parallel for
        for (Index id = 0; id < vDrawables.size(); ++id) {
            if (isAnimable(id)) {
                vAnimables[id]->stopAnimation();
            }
        }

        vAnimationsPaused = false;
        vAnimationsRunning = false;

        this->updateGL();
    }
}

NVL_INLINE bool Canvas::animationsPaused() const
{
    return vAnimationsPaused;
}

NVL_INLINE bool Canvas::animationsRunning() const
{
    return vAnimationsRunning;
}

NVL_INLINE const std::vector<Canvas::PickingData>& Canvas::pickingDataPool() const
{
    return vPickingDataPool;
}

NVL_INLINE std::vector<Canvas::PickingData>& Canvas::pickingDataPool()
{
    return vPickingDataPool;
}

NVL_INLINE const Canvas::PickingData& Canvas::pickingData(const int name) const
{
    return vPickingDataPool.at(static_cast<Index>(name));
}

NVL_INLINE Canvas::PickingData& Canvas::pickingData(const int name)
{
    return vPickingDataPool.at(static_cast<Index>(name));
}

NVL_INLINE Canvas::PickingData::PickingData():
    identifier(PICKING_NONE)
{

}

NVL_INLINE Index Canvas::PickingData::value(const Index& pos) const
{
    return values[pos];
}

NVL_INLINE void Canvas::PickingData::setValue(const Index& pos, const Index& value)
{
    values[pos] = value;
}

NVL_INLINE void Canvas::PickingData::addValue(const Index& value)
{
    values.push_back(value);
}

NVL_INLINE bool Canvas::PickingData::isEmpty() const
{
    return identifier == PICKING_NONE;
}

}

#endif
