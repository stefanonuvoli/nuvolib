#include "qglviewer_canvas.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED
#ifdef NVL_QGLVIEWER_LOADED

#include <nvl/math/alignedbox.h>

#include <nvl/utilities/vector_utils.h>

namespace nvl {

NVL_INLINE QGLViewerObject::QGLViewerObject(QCanvas* canvas, QWidget* parent) :
    QGLViewer(parent),
    vCanvas(canvas),
    vBackgroundColor(Color(255,255,255))
{

}

NVL_INLINE void QGLViewerObject::init()
{
    QGLViewer::setMouseTracking(true);
    QGLViewer::camera()->frame()->setSpinningSensitivity(100.0);

    QGLViewer::setMouseBinding(Qt::NoModifier, Qt::LeftButton, QGLViewer::CAMERA, QGLViewer::ROTATE);
    QGLViewer::setMouseBinding(Qt::NoModifier, Qt::RightButton, QGLViewer::CAMERA, QGLViewer::TRANSLATE);
    QGLViewer::setMouseBinding(Qt::NoModifier, Qt::MiddleButton, QGLViewer::CAMERA, QGLViewer::ZOOM);
    QGLViewer::setWheelBinding(Qt::NoModifier, QGLViewer::CAMERA, QGLViewer::ZOOM);

    QGLViewer::setMouseBinding(Qt::AltModifier, Qt::LeftButton, QGLViewer::FRAME, QGLViewer::ROTATE);
    QGLViewer::setMouseBinding(Qt::AltModifier, Qt::RightButton, QGLViewer::FRAME, QGLViewer::TRANSLATE);
    QGLViewer::setMouseBinding(Qt::AltModifier, Qt::MiddleButton, QGLViewer::FRAME, QGLViewer::NO_MOUSE_ACTION);
    QGLViewer::setWheelBinding(Qt::AltModifier, QGLViewer::CAMERA, QGLViewer::NO_MOUSE_ACTION);

    QGLViewer::setMouseBinding(Qt::ControlModifier, Qt::LeftButton, QGLViewer::FRAME, QGLViewer::NO_MOUSE_ACTION);
    QGLViewer::setMouseBinding(Qt::ControlModifier, Qt::RightButton, QGLViewer::FRAME, QGLViewer::NO_MOUSE_ACTION);
    QGLViewer::setMouseBinding(Qt::ControlModifier, Qt::MiddleButton, QGLViewer::FRAME, QGLViewer::NO_MOUSE_ACTION);
    QGLViewer::setWheelBinding(Qt::ControlModifier, QGLViewer::CAMERA, QGLViewer::NO_MOUSE_ACTION);

    QGLViewer::setMouseBinding(Qt::ControlModifier, Qt::LeftButton, SELECT);
    QGLViewer::setMouseBinding(Qt::ShiftModifier, Qt::LeftButton, SELECT);

    setManipulatedFrame(&vMovableFrame);
}

NVL_INLINE void QGLViewerObject::draw()
{
    QGLViewer::setBackgroundColor(vBackgroundColor.toQColor());

    for (Index i = 0; i < vCanvas->drawableNumber(); ++i) {
        if (vCanvas->drawable(i)->isVisible()) {
            if (vCanvas->isFrameable(i)) {
                vCanvas->frameable(i)->loadFrame();
                vCanvas->drawable(i)->draw();
                vCanvas->frameable(i)->unloadFrame();
            }
            else {
                vCanvas->drawable(i)->draw();
            }
        }
    }
}

NVL_INLINE void QGLViewerObject::drawWithNames()
{
    for (Index i = 0; i < vCanvas->drawableNumber(); ++i) {
        if (vCanvas->drawable(i)->isVisible() && vCanvas->isPickable(i)) {
            if (vCanvas->isFrameable(i)) {
                vCanvas->frameable(i)->loadFrame();
                vCanvas->pickable(i)->drawWithNames(vCanvas, i);
                vCanvas->frameable(i)->unloadFrame();
            }
            else {
                vCanvas->pickable(i)->drawWithNames(vCanvas, i);
            }
        }
    }
}

NVL_INLINE void QGLViewerObject::animate()
{
    bool animationExecuted = false;
    for (Index i = 0; i < vCanvas->drawableNumber(); ++i) {
        if (vCanvas->isAnimable(i)) {
            animationExecuted |= vCanvas->animable(i)->animate();
        }
    }

    if (!animationExecuted) {
        vCanvas->stopAnimations();
    }
}

NVL_INLINE qglviewer::ManipulatedFrame& QGLViewerObject::movableFrame()
{
    return vMovableFrame;
}

NVL_INLINE void QGLViewerObject::setBackgroundColor(const Color& color)
{
    vBackgroundColor = color;
}

NVL_INLINE const Color& QGLViewerObject::backgroundColor() const
{
    return vBackgroundColor;
}

NVL_INLINE void QGLViewerObject::endSelection(const QPoint& qglPoint2D)
{
    glFlush();

    std::vector<int> names;

    GLint nbHits = glRenderMode(GL_RENDER);

    if (nbHits > 0) {
        GLuint* buffer = selectBuffer();

        std::vector<Index> sortingVector(nbHits);
        for (Index i = 0; i < sortingVector.size(); ++i) {
            sortingVector[i] = i;
        }

        BufferSortingStruct sortingStruct(buffer);
        std::sort(sortingVector.begin(), sortingVector.end(), sortingStruct);

        names.resize(nbHits);
        for (int i = 0; i < nbHits; ++i) {
            names[i] = buffer[4 * sortingVector[i] + 3];
        }

        setSelectedName(names[0]);
    }


    qglviewer::Vec qglOrigin, qglDirection, qglPoint3D;
    Point3d lineOrigin(qglOrigin.x, qglOrigin.y, qglOrigin.z);
    Vector3d lineDirection(qglDirection.x, qglDirection.y, qglDirection.z);
    Point2d point2D(qglPoint2D.x(), qglPoint2D.y());
    Point3d point3D(qglPoint3D.x, qglPoint3D.y, qglPoint3D.z);

    bool found = false;

    if (!names.empty()) {
        camera()->convertClickToLine(qglPoint2D, qglOrigin, qglDirection);
        qglPoint3D = camera()->pointUnderPixel(qglPoint2D, found);

        lineOrigin = Point3d(qglOrigin.x, qglOrigin.y, qglOrigin.z);
        lineDirection = Vector3d(qglDirection.x, qglDirection.y, qglDirection.z);
        point2D = Point2d(qglPoint2D.x(), qglPoint2D.y());
        point3D = Point3d(qglPoint3D.x, qglPoint3D.y, qglPoint3D.z);
    }

    emit postSelection(qglPoint2D);
    emit signal_canvasPicking(names, point2D, found, point3D, lineOrigin, lineDirection);
}

}

#endif
#endif
#endif
