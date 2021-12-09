#ifndef NVL_VIEWER_QGL_RAMP_SHADER_H
#define NVL_VIEWER_QGL_RAMP_SHADER_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/gl/qgl_base_shader.h>

namespace nvl {

class QGLRampShader : public QGLBaseShader
{

public:

    QGLRampShader();
    virtual ~QGLRampShader() = default;

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

#include "qgl_ramp_shader.cpp"

#endif // NVL_VIEWER_QGL_RAMP_SHADER_H
