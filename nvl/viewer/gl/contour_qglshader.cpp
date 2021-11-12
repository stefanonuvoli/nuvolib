#include "contour_qglshader.h"

#include <nvl/viewer/gl/draw_gl.h>

namespace nvl {

NVL_INLINE ContourQGLShader::ContourQGLShader() :
    vShaderProgram(nullptr)
{

}

NVL_INLINE ContourQGLShader::~ContourQGLShader()
{
    unload();
}

NVL_INLINE bool ContourQGLShader::load(QGLContext* context)
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

NVL_INLINE void ContourQGLShader::unload() {
    if (isLoaded()) {
        delete vShaderProgram;
        vShaderProgram = nullptr;
    }
}

NVL_INLINE bool ContourQGLShader::isLoaded() const
{
    return vShaderProgram != nullptr;
}

NVL_INLINE void ContourQGLShader::bind()
{
    assert(isLoaded());

    vShaderProgram->bind();
}

NVL_INLINE void ContourQGLShader::release()
{
    assert(isLoaded());
    vShaderProgram->release();
}

NVL_INLINE void ContourQGLShader::initGL()
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

NVL_INLINE void ContourQGLShader::postGL()
{
    glDisable(GL_BLEND);
    glDepthRange (0.0, 1.0);
}

NVL_INLINE void ContourQGLShader::initFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glBegin(GL_POLYGON);
}

NVL_INLINE void ContourQGLShader::postFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glEnd();
}

NVL_INLINE void ContourQGLShader::initVertex(const Index& vId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
}

NVL_INLINE void ContourQGLShader::postVertex(const Index& vId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
}

NVL_INLINE void ContourQGLShader::addVertex(const Index& vId, const Point3d& p)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
    glVertex(p);
}

NVL_INLINE void ContourQGLShader::setAttribute(int location, double value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, value);
}

NVL_INLINE void ContourQGLShader::setUniform(int location, double value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void ContourQGLShader::setUniform(const std::string &name, double value)
{
    vShaderProgram->setUniformValue(name.c_str(), static_cast<GLfloat>(value));
}

NVL_INLINE void ContourQGLShader::setAttribute(const std::string &name, double value)
{
    vShaderProgram->setAttributeValue(name.c_str(), value);
}

NVL_INLINE int ContourQGLShader::attributeLocation(const std::string& name) const
{
    assert(isLoaded());
    return vShaderProgram->attributeLocation(name.c_str());
}

NVL_INLINE int ContourQGLShader::uniformLocation(const std::string& name) const
{
    return vShaderProgram->uniformLocation(name.c_str());
}

}
