#include "qgl_ramp_shader.h"

#include <nvl/viewer/gl/gl_draw.h>

namespace nvl {

NVL_INLINE QGLRampShader::QGLRampShader() :
    vShaderProgram(nullptr)
{

}

NVL_INLINE QGLRampShader::~QGLRampShader()
{
    unload();
}

NVL_INLINE bool QGLRampShader::load(QGLContext* context)
{
    unload();

    vShaderProgram = new QGLShaderProgram(context);

    bool ret = true;
    ret &= vShaderProgram->addShaderFromSourceFile(QGLShader::Vertex,":/shaders/ramp.vert");
    ret &= vShaderProgram->addShaderFromSourceFile(QGLShader::Fragment,":/shaders/ramp.frag");
    ret &= vShaderProgram->link();
    assert(ret && "Error compiling ramp shader.");

    if (!ret)
        vShaderProgram = nullptr;

    return ret;
}

NVL_INLINE void QGLRampShader::unload() {
    if (!isLoaded())
        return;

    delete vShaderProgram;
    vShaderProgram = nullptr;
}

NVL_INLINE bool QGLRampShader::isLoaded() const
{
    return vShaderProgram != nullptr;
}

NVL_INLINE void QGLRampShader::bind()
{
    assert(isLoaded());
    vShaderProgram->bind();
}

NVL_INLINE void QGLRampShader::release()
{
    assert(isLoaded());
    vShaderProgram->release();
}

NVL_INLINE void QGLRampShader::initGL()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthRange (0.01, 1.0);
    glDepthFunc(GL_LEQUAL);

    setUniform(uniformLocation("value_min"), 0.0f);
    setUniform(uniformLocation("value_max"), 1.0f);
    setUniform(uniformLocation("ramp_alpha"), 0.8f);
}

NVL_INLINE void QGLRampShader::postGL()
{
    glDisable(GL_BLEND);
    glDepthRange (0.0, 1.0);
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

NVL_INLINE void QGLRampShader::setAttribute(int location, double value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, value);
}

NVL_INLINE void QGLRampShader::setUniform(int location, double value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLRampShader::setUniform(const std::string &name, double value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(name.c_str(), static_cast<GLfloat>(value));
}

NVL_INLINE void QGLRampShader::setAttribute(const std::string &name, double value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(name.c_str(), value);
}

NVL_INLINE int QGLRampShader::attributeLocation(const std::string& name) const
{
    assert(isLoaded());
    return vShaderProgram->attributeLocation(name.c_str());
}

NVL_INLINE int QGLRampShader::uniformLocation(const std::string& name) const
{
    assert(isLoaded());
    return vShaderProgram->uniformLocation(name.c_str());
}

}
