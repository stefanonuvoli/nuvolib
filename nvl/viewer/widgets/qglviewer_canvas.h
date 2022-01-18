#ifndef NVL_VIEWER_QGLVIEWER_CANVAS_H
#define NVL_VIEWER_QGLVIEWER_CANVAS_H

#include <nvl/nuvolib.h>

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include <nvl/viewer/widgets/qcanvas.h>

#include <QVBoxLayout>
#include <QGLContext>

namespace nvl {

class QGLViewerObject;

class QGLViewerCanvas : public QCanvas
{
    Q_OBJECT

public:

    QGLViewerCanvas(QWidget* parent = nullptr);
    ~QGLViewerCanvas();


    /* Canvas methods */

    QGLContext* qglContext() override;

    void updateGL() override;

    void setMovableFrame(const Affine3d& frame) override;

    void startAnimations() override;
    void stopAnimations() override;
    void setTargetFPS(double fps) override;
    double targetFPS() override;

    void setFPSDisplayed(bool display = true) override;
    bool isFPSDisplayed() override;
    void setAxisDisplayed(bool display = true) override;
    bool isAxisDisplayed() override;
    void setBackgroundColor(const Color& color) override;
    const Color& backgroundColor() const override;

    void fitScene(bool visibleObjects = true) override;
    void fitSceneSphere(const Point3d& center, const double radius) override;
    void fitSceneBoundingBox(const AlignedBox3d& bbox) override;
    void fitSceneBoundingBox(const Point3d& min, const Point3d& max) override;

    unsigned int screenWidth() override;
    unsigned int screenHeight() override;

    Point3d cameraSceneCenter() const override;
    void setCameraSceneCenter(const Point3d& cameraSceneCenter) override;
    double cameraSceneRadius() const override;
    void setCameraSceneRadius(const double radius) override;
    Point3d cameraPosition() const override;
    void setCameraPosition(const Point3d& cameraPosition) override;
    virtual Vector3d cameraViewDirection() const override;
    virtual void setCameraViewDirection(const Vector3d& viewDirection) const override;
    virtual Vector3d cameraUpVector() const override;
    virtual void setCameraUpVector(const Vector3d& upVector) const override;
    virtual Vector3d cameraRightVector() const override;
    virtual Quaterniond cameraOrientation() const override;
    virtual void setCameraOrientation(const Quaterniond& orientation) const override;
    void setCameraLookAt(const Point3d& point) override;
    virtual Matrix44d cameraProjectionMatrix() const override;
    virtual Matrix44d cameraModelViewMatrix() const override;
    virtual Matrix44d cameraModelViewProjectionMatrix() const override;

private:

    void initialize();
    void connectSignals();

    QVBoxLayout* vLayout;

    QGLViewerObject* vQGLViewerObject;

    double vTargetFps;
    double scaleSensitivity;

protected:

    void wheelEvent(QWheelEvent *event) override;


public Q_SLOTS:

    void slot_canvasFpsDisplayedChanged(bool display);
    void slot_canvasAxisDisplayedChanged(bool display);
    void slot_canvasMovableFrameChanged();

};

}

#include <nvl/viewer/widgets/qglviewer_object.h>

#endif
#endif

#include "qglviewer_canvas.cpp"

#endif // NVL_VIEWER_QGLVIEWER_CANVAS_H
