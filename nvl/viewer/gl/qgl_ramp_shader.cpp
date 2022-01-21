#include "qgl_ramp_shader.h"

#ifdef NVL_QGLVIEWER_LOADED

#include <nvl/viewer/gl/gl_draw.h>

namespace nvl {

NVL_INLINE QGLRampShader::QGLRampShader() :
    vShaderProgram(nullptr)
{

}

NVL_INLINE bool QGLRampShader::load(QGLContext* context)
{
    return QGLBaseShader::load(context, ":/shaders/ramp.vert", ":/shaders/ramp.frag");
}

NVL_INLINE void QGLRampShader::initGL()
{
    setUniform("value_min", 0.0f);
    setUniform("value_max", 1.0f);
    setUniform("ramp_alpha", 0.8f);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthRange(0.0001, 1.0);
    glDepthFunc(GL_LEQUAL);
}

NVL_INLINE void QGLRampShader::postGL()
{
    glDisable(GL_BLEND);
    glDepthRange(0.0, 1.0);
}

NVL_INLINE void QGLRampShader::initFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glBegin(GL_POLYGON);
}

NVL_INLINE void QGLRampShader::postFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glEnd();
}

NVL_INLINE void QGLRampShader::initVertex(const Index& vId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
}

NVL_INLINE void QGLRampShader::postVertex(const Index& vId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
}

NVL_INLINE void QGLRampShader::addVertex(const Index& vId, const Point3d& p)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
    glVertex(p);
}

}

#endif
