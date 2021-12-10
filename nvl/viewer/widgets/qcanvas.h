#ifndef NVL_VIEWER_QCANVAS_H
#define NVL_VIEWER_QCANVAS_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/widgets/canvas.h>

#include <nvl/viewer/gl/opengl_headers.h> //Needed to load glew

#include <QWidget>
#include <QGLContext>

namespace nvl {

class QCanvas : public QWidget, public Canvas
{
    Q_OBJECT

public:

    /* Constructors/destructors */

    QCanvas(QWidget* parent = nullptr);
    virtual ~QCanvas() = default;


    /* Methods */

    virtual Index addDrawable(Drawable* drawable, const std::string& name = "", bool listable = true) override;
    virtual bool removeDrawable(const Index& id) override;
    virtual bool removeDrawable(Drawable* drawable) override;

    virtual void setDrawableName(const Index& id, const std::string& name) override;


    /* Abstract methods */

    virtual QGLContext* qglContext() = 0;

    virtual void startAnimations() override;
    virtual void pauseAnimations() override;
    virtual void stopAnimations() override;


public Q_SLOTS:

    void slot_canvasPicking(
            const std::vector<int>& names,
            const Point2d& point2D,
            const bool& found,
            const Point3d& point3D,
            const Point3d& lineOrigin,
            const Vector3d& lineDirection);


Q_SIGNALS:

    void signal_fpsDisplayedChanged(bool display);
    void signal_axisDisplayedChanged(bool display);
    void signal_backgroundColorChanged(const Color& color);
    void signal_drawableListChanged(const std::vector<Drawable*>& list);
    void signal_drawableAdded(const Index& id, Drawable* drawable);
    void signal_drawableRemoved(const Index& id, Drawable* drawable);
    void signal_movableFrameChanged(const Affine3d& affine);
    void signal_animationStarted();
    void signal_animationPaused();
    void signal_animationStopped();
    void signal_canvasPicking(
            const std::vector<PickingData>& data,
            const Point2d& point2D,
            const bool& found,
            const Point3d& point3D,
            const Point3d& lineOrigin,
            const Vector3d& lineDirection);

};

}

#include "qcanvas.cpp"

#endif // NVL_VIEWER_QCANVAS_H
