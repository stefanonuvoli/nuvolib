/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_QGLVIEWER_OBJECT_H
#define NVL_VIEWER_QGLVIEWER_OBJECT_H

#include <nvl/nuvolib.h>

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED
#ifdef NVL_QGLVIEWER_LOADED

#include <nvl/viewer/gl/opengl_headers.h>
#ifdef WIN32
#include "windows.h"
#endif

#include <QWidget>

#include <QGLViewer/qglviewer.h>
#include <QGLViewer/manipulatedCameraFrame.h>
#include <QGLWidget>
#include <QKeyEvent>

#include <vector>

#include <nvl/math/point.h>

#include <nvl/utilities/color.h>

#include <nvl/viewer/widgets/qcanvas.h>

namespace nvl {

class QGLViewerObject : public QGLViewer
{
    Q_OBJECT

public:

    QGLViewerObject(QCanvas* canvas, QWidget* parent = nullptr);

    virtual ~QGLViewerObject() = default;

    virtual void init() override;
    virtual void draw() override;
    virtual void drawWithNames() override;
    virtual void animate() override;

    void setBackgroundColor(const Color& color);
    const Color& backgroundColor() const;

    void fitScene(bool visibleObjects = true);

    qglviewer::ManipulatedFrame& movableFrame();

    void endSelection(const QPoint& qglPoint) override;


Q_SIGNALS:

    void signal_canvasPicking(
            const std::vector<int> name,
            const Point2d& point2D,
            const bool& found,
            const Point3d& point3D,
            const Point3d& lineOrigin,
            const Vector3d& lineDirection);

private:

    QCanvas* vCanvas;

    Color vBackgroundColor;

    qglviewer::ManipulatedFrame vMovableFrame;

    struct BufferSortingStruct {
        BufferSortingStruct(GLuint* buffer) :
            vBuffer(buffer) {}

        bool operator() (const Index& lhs, const Index& rhs)
        {
            return vBuffer[4 * lhs + 1] < vBuffer[4 * rhs + 1];
        }

        GLuint* vBuffer;
    };

};

}

#endif
#endif
#endif

#include "qglviewer_object.cpp"

#endif // NVL_VIEWER_QGLVIEWER_OBJECT_H
