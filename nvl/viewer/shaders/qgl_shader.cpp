#include "qgl_shader.h"

#ifdef NVL_QGLVIEWER_LOADED

namespace nvl {

NVL_INLINE QGLShader::QGLShader() :
    vShaderProgram(nullptr)
{

}

NVL_INLINE QGLShader::~QGLShader()
{
    unload();
}

NVL_INLINE void QGLShader::unload() {
    if (isLoaded()) {
        delete vShaderProgram;
        vShaderProgram = nullptr;
    }
}

NVL_INLINE bool QGLShader::isLoaded() const
{
    return vShaderProgram != nullptr;
}

NVL_INLINE void QGLShader::bind()
{
    assert(isLoaded());
    vShaderProgram->bind();
}

NVL_INLINE void QGLShader::release()
{
    assert(isLoaded());
    vShaderProgram->release();
}

NVL_INLINE int QGLShader::uniformLocation(const std::string& name) const
{
    assert(isLoaded());
    return vShaderProgram->uniformLocation(name.c_str());
}

NVL_INLINE void QGLShader::setUniform(const int& location, const double& value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLShader::setUniform(const int& location, const float& value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLShader::setUniform(const int& location, const int& value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, static_cast<GLint>(value));
}

NVL_INLINE void QGLShader::setUniform(const int& location, const Color& value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, value.toQColor());
}

NVL_INLINE void QGLShader::setUniform(const std::string& name, const double& value)
{
    assert(isLoaded());
    setUniform(uniformLocation(name), value);
}

NVL_INLINE void QGLShader::setUniform(const std::string& name, const float& value)
{
    assert(isLoaded());
    setUniform(uniformLocation(name), value);
}

NVL_INLINE void QGLShader::setUniform(const std::string& name, const int& value)
{
    assert(isLoaded());
    setUniform(uniformLocation(name), value);
}

NVL_INLINE void QGLShader::setUniform(const std::string& name, const Color& value)
{
    assert(isLoaded());
    setUniform(uniformLocation(name), value);
}

NVL_INLINE int QGLShader::attributeLocation(const std::string& name) const
{
    assert(isLoaded());
    return vShaderProgram->attributeLocation(name.c_str());
}

NVL_INLINE void QGLShader::setAttribute(const int& location, const double& value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLShader::setAttribute(const int& location, const float& value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLShader::setAttribute(const int& location, const int& value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, static_cast<GLint>(value));
}

NVL_INLINE void QGLShader::setAttribute(const int& location, const Color& value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, value.toQColor());
}

NVL_INLINE void QGLShader::setAttribute(const std::string& name, const double& value)
{
    assert(isLoaded());
    setAttribute(attributeLocation(name), value);
}

NVL_INLINE void QGLShader::setAttribute(const std::string& name, const float& value)
{
    assert(isLoaded());
    setAttribute(attributeLocation(name), value);
}

NVL_INLINE void QGLShader::setAttribute(const std::string& name, const int& value)
{
    assert(isLoaded());
    setAttribute(attributeLocation(name), value);
}

NVL_INLINE void QGLShader::setAttribute(const std::string& name, const Color& value)
{
    assert(isLoaded());
    setAttribute(attributeLocation(name), value);
}

NVL_INLINE bool QGLShader::load(
        QGLContext* context,
        const std::string& vertexShader,
        const std::string& fragmentShader)
{
    createProgram(context);

    bool ret = true;
    ret &= vShaderProgram->addShaderFromSourceFile(::QGLShader::Vertex, vertexShader.c_str());
    ret &= vShaderProgram->addShaderFromSourceFile(::QGLShader::Fragment, fragmentShader.c_str());

    ret &= link();

    return ret;
}

bool NVL_INLINE QGLShader::loadVertexShader(
    QGLContext* context,
    const std::string& vertexShader)
{
    createProgram(context);

    bool ret = true;
    ret &= vShaderProgram->addShaderFromSourceFile(::QGLShader::Vertex, vertexShader.c_str());

    ret &= link();

    return ret;
}

bool NVL_INLINE QGLShader::loadFragmentShader(
    QGLContext* context,
    const std::string& fragmentShader)
{
    createProgram(context);

    bool ret = true;
    ret &= vShaderProgram->addShaderFromSourceFile(::QGLShader::Fragment, fragmentShader.c_str());

    ret &= link();

    return ret;
}

void NVL_INLINE QGLShader::createProgram(
        QGLContext* context)
{
    unload();

    vShaderProgram = new QGLShaderProgram(context);
}

bool NVL_INLINE QGLShader::link()
{
    bool ret = true;

    ret &= vShaderProgram->link();
    assert(ret && "Error compiling shader.");

    if (!ret)
        vShaderProgram = nullptr;

    return ret;
}

}

#endif
