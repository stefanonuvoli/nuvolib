#ifndef NVL_VIEWER_QGL_CONTOUR_SHADER_H
#define NVL_VIEWER_QGL_CONTOUR_SHADER_H

#include <nvl/nuvolib.h>

#ifdef NVL_QGLVIEWER_LOADED

#include <nvl/viewer/gl/qgl_base_shader.h>

namespace nvl {

class QGLContourShader : public QGLBaseShader
{

public:

    QGLContourShader();
    virtual ~QGLContourShader() = default;

    bool load(QGLContext* context);

    void initGL() override;
    void postGL() override;

    void initFace(const Index& fId) override;
    void postFace(const Index& fId) override;

    void initVertex(const Index& vId) override;
    void postVertex(const Index& vId) override;

    void addVertex(const Index& vId, const Point3d& p) override;

protected:

    QGLShaderProgram* vShaderProgram;

};

}

#endif

#include "qgl_contour_shader.cpp"

#endif // NVL_VIEWER_QGL_CONTOUR_SHADER_H
