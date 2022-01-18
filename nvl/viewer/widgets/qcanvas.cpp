#include "qcanvas.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include <nvl/utilities/vector_utils.h>

namespace nvl {

NVL_INLINE QCanvas::QCanvas(QWidget* parent) :
    QWidget(parent),
    Canvas()
{

}

NVL_INLINE Index QCanvas::addDrawable(Drawable* drawable, const std::string& name, const bool listable)
{
    Index id = Canvas::addDrawable(drawable, name, listable);

    emit signal_drawableAdded(id, drawable);
    emit signal_drawableListChanged(vDrawables);

    return id;
}

NVL_INLINE bool QCanvas::removeDrawable(const Index& id)
{
    if (id >= vDrawables.size())
        return false;

    Drawable* drawable = vDrawables[id];

    bool result = Canvas::removeDrawable(id);

    if (result) {
        emit signal_drawableRemoved(id, drawable);
        emit signal_drawableListChanged(vDrawables);
    }

    return result;
}

NVL_INLINE bool QCanvas::removeDrawable(Drawable* drawable)
{
    std::vector<Drawable*>::iterator it = std::find(vDrawables.begin(), vDrawables.end(), drawable);
    if (it != vDrawables.end()) {
        return removeDrawable(std::distance(vDrawables.begin(), it));
    }
    else {
        return false;
    }
}

NVL_INLINE void QCanvas::setDrawableName(const Index& id, const std::string& name)
{
    Canvas::setDrawableName(id, name);

    emit signal_drawableListChanged(vDrawables);
}

NVL_INLINE void QCanvas::startAnimations()
{    
    Canvas::startAnimations();

    emit signal_animationStarted();
}

NVL_INLINE void QCanvas::pauseAnimations()
{
    Canvas::pauseAnimations();

    emit signal_animationPaused();
}
NVL_INLINE void QCanvas::stopAnimations()
{
    Canvas::stopAnimations();

    emit signal_animationStopped();
}

NVL_INLINE void QCanvas::slot_canvasPicking(
        const std::vector<int>& names,
        const Point2d& point2D,
        const bool& found,
        const Point3d& point3D,
        const Point3d& lineOrigin,
        const Vector3d& lineDirection)
{
    std::vector<PickingData> data;
    for (int name : names) {
        if (name >= 0) {
            assert(static_cast<Index>(name) < vPickingDataPool.size());

            const PickingData& picked = this->vPickingDataPool[static_cast<Index>(name)];
            data.push_back(picked);
        }
    }
    vPickingDataPool.clear();

    emit signal_canvasPicking(data, point2D, found, point3D, lineOrigin, lineDirection);
}

}

#endif
#endif
