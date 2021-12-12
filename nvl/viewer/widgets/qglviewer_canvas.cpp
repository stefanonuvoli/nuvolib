#include "qglviewer_canvas.h"

#include <nvl/math/quaternion.h>
#include <nvl/math/constants.h>

#define NVL_QGLCANVAS_DEFAULT_SCALING_SENSITIVITY 1.05
#define NVL_QGLCANVAS_SCALING_DEGREE 120.0

namespace nvl {

NVL_INLINE QGLViewerCanvas::QGLViewerCanvas(QWidget* parent) :
    QCanvas(parent),
    vLayout(new QVBoxLayout()),
    vQGLViewerObject(new QGLViewerObject(this, this)),
    scaleSensitivity(NVL_QGLCANVAS_DEFAULT_SCALING_SENSITIVITY)
{
    this->setLayout(vLayout);
    vLayout->addWidget(vQGLViewerObject);

    setTargetFPS(60);

    connectSignals();
}

NVL_INLINE QGLViewerCanvas::~QGLViewerCanvas()
{
    delete vLayout;
    delete vQGLViewerObject;
}

NVL_INLINE QGLContext* QGLViewerCanvas::qglContext()
{
    return vQGLViewerObject->context();
}

NVL_INLINE void QGLViewerCanvas::updateGL()
{
    vQGLViewerObject->updateGL();
}

NVL_INLINE void QGLViewerCanvas::setMovableFrame(const Affine3d& frame)
{
    QCanvas::setMovableFrame(frame);

    vQGLViewerObject->movableFrame().setFromMatrix(frame.matrix().data());
}

NVL_INLINE void QGLViewerCanvas::startAnimations()
{
    if (!vQGLViewerObject->animationIsStarted()) {
        vQGLViewerObject->startAnimation();
    }
    QCanvas::startAnimations();
}

NVL_INLINE void QGLViewerCanvas::stopAnimations()
{
    if (vQGLViewerObject->animationIsStarted()) {
        vQGLViewerObject->stopAnimation();
    }
    QCanvas::stopAnimations();
}

NVL_INLINE void QGLViewerCanvas::setTargetFPS(double fps)
{
    vTargetFps = fps;
    vQGLViewerObject->setAnimationPeriod(static_cast<int>(std::ceil((1000.0/vTargetFps))));
}

NVL_INLINE double QGLViewerCanvas::targetFPS()
{
    return vTargetFps;
}

NVL_INLINE void QGLViewerCanvas::setFPSDisplayed(bool display)
{
    vQGLViewerObject->setFPSIsDisplayed(display);
}

NVL_INLINE bool QGLViewerCanvas::isFPSDisplayed()
{
    return vQGLViewerObject->FPSIsDisplayed();
}

NVL_INLINE void QGLViewerCanvas::setAxisDisplayed(bool display)
{
    vQGLViewerObject->setAxisIsDrawn(display);
}

NVL_INLINE bool QGLViewerCanvas::isAxisDisplayed()
{
    return vQGLViewerObject->axisIsDrawn();
}

NVL_INLINE void QGLViewerCanvas::setBackgroundColor(const Color& color)
{
    vQGLViewerObject->setBackgroundColor(color);
    emit signal_backgroundColorChanged(color);
}

NVL_INLINE const Color& QGLViewerCanvas::backgroundColor() const
{
    return vQGLViewerObject->backgroundColor();
}
NVL_INLINE void QGLViewerCanvas::fitScene(bool visibleObjects)
{
    AlignedBox3d boundingBox;

    for (const Drawable* drawable : this->drawables()) {
        if (!visibleObjects || drawable->isVisible()) {
            Vector3d sceneRadiusVector(drawable->sceneRadius(), drawable->sceneRadius(), drawable->sceneRadius());
            Point3d p1 = drawable->sceneCenter() + sceneRadiusVector;
            Point3d p2 = drawable->sceneCenter() - sceneRadiusVector;

            boundingBox.extend(p1);
            boundingBox.extend(p2);
        }
    }

    Point3d sceneCenter;
    double sceneRadius;
    if (boundingBox.isNull()) {
        sceneCenter = Point3d(0, 0, 0);
        sceneRadius = 1;
    }
    else {
        sceneCenter = boundingBox.center();
        sceneRadius = boundingBox.diagonal().norm() / 2.0;
    }

    if (sceneRadius <= 0) {
        sceneRadius = EPSILON;
    }

    vQGLViewerObject->setSceneCenter(qglviewer::Vec(sceneCenter.x(), sceneCenter.y(), sceneCenter.z()));
    vQGLViewerObject->setSceneRadius(sceneRadius * 0.6);
    vQGLViewerObject->showEntireScene();
}

NVL_INLINE void QGLViewerCanvas::fitSceneSphere(const Point3d& center, const double radius)
{
    vQGLViewerObject->camera()->fitSphere(qglviewer::Vec(center.x(), center.y(), center.z()), radius);
}

NVL_INLINE void QGLViewerCanvas::fitSceneBoundingBox(const AlignedBox3d& bbox)
{
    this->fitSceneBoundingBox(bbox.min(), bbox.max());
}

NVL_INLINE void QGLViewerCanvas::fitSceneBoundingBox(const Point3d& min, const Point3d& max)
{
    vQGLViewerObject->camera()->fitBoundingBox(
                qglviewer::Vec(min.x(), min.y(), min.z()),
                qglviewer::Vec(max.x(), max.y(), max.z()));
}

NVL_INLINE unsigned int QGLViewerCanvas::screenWidth()
{
    return vQGLViewerObject->camera()->screenWidth();
}

NVL_INLINE unsigned int QGLViewerCanvas::screenHeight()
{
    return vQGLViewerObject->camera()->screenHeight();
}

NVL_INLINE Point3d QGLViewerCanvas::cameraSceneCenter() const
{
    qglviewer::Vec center = vQGLViewerObject->sceneCenter();
    return Point3d(center.x, center.y, center.z);
}

NVL_INLINE void QGLViewerCanvas::setCameraSceneCenter(const Point3d& center)
{
    vQGLViewerObject->setSceneCenter(qglviewer::Vec(center.x(), center.y(), center.z()));
}

NVL_INLINE double QGLViewerCanvas::cameraSceneRadius() const
{
    return static_cast<double>(vQGLViewerObject->sceneRadius());
}

NVL_INLINE void QGLViewerCanvas::setCameraSceneRadius(double radius)
{
    vQGLViewerObject->setSceneRadius(radius);
}

NVL_INLINE Point3d QGLViewerCanvas::cameraPosition() const
{
    qglviewer::Vec position = vQGLViewerObject->camera()->position();
    return Point3d(position.x, position.y, position.z);
}

NVL_INLINE void QGLViewerCanvas::setCameraPosition(const Point3d& cameraPosition)
{
    vQGLViewerObject->camera()->setPosition(qglviewer::Vec(cameraPosition.x(), cameraPosition.y(), cameraPosition.z()));
}

NVL_INLINE Vector3d QGLViewerCanvas::cameraViewDirection() const
{
    qglviewer::Vec viewDirection = vQGLViewerObject->camera()->viewDirection();
    return Vector3d(viewDirection.x, viewDirection.y, viewDirection.z);
}

NVL_INLINE void QGLViewerCanvas::setCameraViewDirection(const Vector3d& viewDirection) const
{
    vQGLViewerObject->camera()->setViewDirection(qglviewer::Vec(viewDirection.x(), viewDirection.y(), viewDirection.z()));
}

NVL_INLINE Vector3d QGLViewerCanvas::cameraUpVector() const
{
    qglviewer::Vec upVector = vQGLViewerObject->camera()->upVector();
    return Vector3d(upVector.x, upVector.y, upVector.z);
}

NVL_INLINE void QGLViewerCanvas::setCameraUpVector(const Vector3d& upVector) const
{
    vQGLViewerObject->camera()->setUpVector(qglviewer::Vec(upVector.x(), upVector.y(), upVector.z()));
}

NVL_INLINE Vector3d QGLViewerCanvas::cameraRightVector() const
{
    qglviewer::Vec rightVector = vQGLViewerObject->camera()->rightVector();
    return Vector3d(rightVector.x, rightVector.y, rightVector.z);
}

NVL_INLINE Quaterniond QGLViewerCanvas::cameraOrientation() const
{
    qglviewer::Quaternion orientation = vQGLViewerObject->camera()->orientation();
    return Quaterniond(orientation[0], orientation[1], orientation[2], orientation[3]);
}

NVL_INLINE void QGLViewerCanvas::setCameraOrientation(const Quaterniond& orientation) const
{
    vQGLViewerObject->camera()->setOrientation(qglviewer::Quaternion(orientation.x(), orientation.y(), orientation.z(), orientation.w()));
}

NVL_INLINE void QGLViewerCanvas::setCameraLookAt(const Point3d& point)
{
    vQGLViewerObject->camera()->lookAt(qglviewer::Vec(point.x(), point.y(), point.z()));
}

NVL_INLINE Matrix44d QGLViewerCanvas::cameraProjectionMatrix() const
{
    GLdouble m[16];
    vQGLViewerObject->camera()->getProjectionMatrix(m);
    return Matrix44d(m).transpose();
}

NVL_INLINE Matrix44d QGLViewerCanvas::cameraModelViewMatrix() const
{
    GLdouble m[16];
    vQGLViewerObject->camera()->getModelViewMatrix(m);
    return Matrix44d(m).transpose();
}

NVL_INLINE Matrix44d QGLViewerCanvas::cameraModelViewProjectionMatrix() const
{
    GLdouble m[16];
    vQGLViewerObject->camera()->getModelViewProjectionMatrix(m);
    return Matrix44d(m).transpose();
}

NVL_INLINE void QGLViewerCanvas::connectSignals()
{
    if (vQGLViewerObject != nullptr) {
        //Connect signals to qglviewer object
        connect(vQGLViewerObject, &QGLViewerObject::FPSIsDisplayedChanged, this, &QGLViewerCanvas::slot_canvasFpsDisplayedChanged);
        connect(vQGLViewerObject, &QGLViewerObject::axisIsDrawnChanged, this, &QGLViewerCanvas::slot_canvasAxisDisplayedChanged);
        connect(&vQGLViewerObject->movableFrame(), &qglviewer::ManipulatedFrame::manipulated, this, &QGLViewerCanvas::slot_canvasMovableFrameChanged);
        connect(vQGLViewerObject, &QGLViewerObject::signal_canvasPicking, this, &QGLViewerCanvas::slot_canvasPicking);
    }
}

NVL_INLINE void QGLViewerCanvas::slot_canvasFpsDisplayedChanged(bool display)
{
    emit signal_fpsDisplayedChanged(display);
}

NVL_INLINE void QGLViewerCanvas::slot_canvasAxisDisplayedChanged(bool display)
{
    emit signal_axisDisplayedChanged(display);
}

NVL_INLINE void QGLViewerCanvas::slot_canvasMovableFrameChanged()
{
    const GLdouble* m = vQGLViewerObject->movableFrame().matrix();
    Matrix44d matrix(m);

    Affine3d frame(matrix);

    this->setMovableFrame(frame);

    emit signal_movableFrameChanged(vMovableFrame);
}

NVL_INLINE void QGLViewerCanvas::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::AltModifier)
    {
        const GLdouble* m = vQGLViewerObject->movableFrame().matrix();
        Matrix44d matrix(m);

        QPoint p = event->angleDelta();
        double v = p.x();

        Affine3d frame(matrix);

        double multiplier = this->scaleSensitivity;

        v = v / 120.0;
        if (v > 0) {
            frame.scale(v * multiplier);
        }
        else if (v < 0) {
            frame.scale(1.0 / (-v * multiplier));
        }

        this->setMovableFrame(frame);

        emit signal_movableFrameChanged(vMovableFrame);
    }

    event->accept();
}

}
