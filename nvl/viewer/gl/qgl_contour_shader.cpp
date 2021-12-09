#include "qgl_contour_shader.h"

#include <nvl/viewer/gl/gl_draw.h>

namespace nvl {

NVL_INLINE QGLContourShader::QGLContourShader() :
    vShaderProgram(nullptr)
{

}

NVL_INLINE QGLContourShader::~QGLContourShader()
{
    unload();
}

NVL_INLINE bool QGLContourShader::load(QGLContext* context)
{
    unload();

    vShaderProgram = new QGLShaderProgram(context);

    bool ret = true;
    ret &= vShaderProgram->addShaderFromSourceFile(QGLShader::Vertex,":/shaders/contour.vert");
    ret &= vShaderProgram->addShaderFromSourceFile(QGLShader::Fragment,":/shaders/contour.frag");
    ret &= vShaderProgram->link();
    assert(ret && "Error compiling contour shader.");

    if (!ret)
        vShaderProgram = nullptr;

    return ret;
}

NVL_INLINE void QGLContourShader::unload() {
    if (isLoaded()) {
        delete vShaderProgram;
        vShaderProgram = nullptr;
    }
}

NVL_INLINE bool QGLContourShader::isLoaded() const
{
    return vShaderProgram != nullptr;
}

NVL_INLINE void QGLContourShader::bind()
{
    assert(isLoaded());

    vShaderProgram->bind();
}

NVL_INLINE void QGLContourShader::release()
{
    assert(isLoaded());
    vShaderProgram->release();
}

NVL_INLINE void QGLContourShader::initGL()
{
    setUniform(uniformLocation("value_min"), 0.0f);
    setUniform(uniformLocation("value_max"), 1.0f);
    setUniform(uniformLocation("stripe_num"), 10.f);
    setUniform(uniformLocation("stripe_width"), 0.6f);
    setUniform(uniformLocation("stripe_alpha"), 0.8f);

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

NVL_INLINE void QGLContourShader::setAttribute(int location, double value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, value);
}

NVL_INLINE void QGLContourShader::setUniform(int location, double value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLContourShader::setUniform(const std::string &name, double value)
{
    vShaderProgram->setUniformValue(name.c_str(), static_cast<GLfloat>(value));
}

NVL_INLINE void QGLContourShader::setAttribute(const std::string &name, double value)
{
    vShaderProgram->setAttributeValue(name.c_str(), value);
}

NVL_INLINE int QGLContourShader::attributeLocation(const std::string& name) const
{
    assert(isLoaded());
    return vShaderProgram->attributeLocation(name.c_str());
}

NVL_INLINE int QGLContourShader::uniformLocation(const std::string& name) const
{
    return vShaderProgram->uniformLocation(name.c_str());
}

}
