#include "qgl_contour_shader.h"

#include <nvl/viewer/gl/gl_draw.h>

namespace nvl {

NVL_INLINE QGLContourShader::QGLContourShader() :
    vShaderProgram(nullptr)
{

}

NVL_INLINE bool QGLContourShader::load(QGLContext* context)
{
    return QGLBaseShader::load(context, ":/shaders/contour.vert", ":/shaders/contour.frag");
}

NVL_INLINE void QGLContourShader::initGL()
{
    setUniform("value_min", 0.0f);
    setUniform("value_max", 1.0f);
    setUniform("stripe_num", 10.f);
    setUniform("stripe_width", 0.6f);
    setUniform("stripe_alpha", 0.8f);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthRange (0.01, 1.0);
    glDepthFunc(GL_LEQUAL);
}

NVL_INLINE void QGLContourShader::postGL()
{
    glDisable(GL_BLEND);
    glDepthRange (0.0, 1.0);
}

NVL_INLINE void QGLContourShader::initFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glBegin(GL_POLYGON);
}

NVL_INLINE void QGLContourShader::postFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glEnd();
}

NVL_INLINE void QGLContourShader::initVertex(const Index& vId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
}

NVL_INLINE void QGLContourShader::postVertex(const Index& vId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
}

NVL_INLINE void QGLContourShader::addVertex(const Index& vId, const Point3d& p)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
    glVertex(p);
}

}
