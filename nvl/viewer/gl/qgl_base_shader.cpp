#include "qgl_base_shader.h"

#ifdef NVL_QGLVIEWER_LOADED

namespace nvl {

NVL_INLINE QGLBaseShader::QGLBaseShader() :
    vShaderProgram(nullptr)
{

}

NVL_INLINE QGLBaseShader::~QGLBaseShader()
{
    unload();
}

NVL_INLINE void QGLBaseShader::unload() {
    if (isLoaded()) {
        delete vShaderProgram;
        vShaderProgram = nullptr;
    }
}

NVL_INLINE bool QGLBaseShader::isLoaded() const
{
    return vShaderProgram != nullptr;
}

NVL_INLINE void QGLBaseShader::bind()
{
    assert(isLoaded());
    vShaderProgram->bind();
}

NVL_INLINE void QGLBaseShader::release()
{
    assert(isLoaded());
    vShaderProgram->release();
}

NVL_INLINE int QGLBaseShader::uniformLocation(const std::string& name) const
{
    assert(isLoaded());
    return vShaderProgram->uniformLocation(name.c_str());
}

NVL_INLINE void QGLBaseShader::setUniform(const int& location, const double& value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLBaseShader::setUniform(const int& location, const float& value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLBaseShader::setUniform(const int& location, const int& value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, static_cast<GLint>(value));
}

NVL_INLINE void QGLBaseShader::setUniform(const int& location, const Color& value)
{
    assert(isLoaded());
    vShaderProgram->setUniformValue(location, value.toQColor());
}

NVL_INLINE void QGLBaseShader::setUniform(const std::string& name, const double& value)
{
    assert(isLoaded());
    setUniform(uniformLocation(name), value);
}

NVL_INLINE void QGLBaseShader::setUniform(const std::string& name, const float& value)
{
    assert(isLoaded());
    setUniform(uniformLocation(name), value);
}

NVL_INLINE void QGLBaseShader::setUniform(const std::string& name, const int& value)
{
    assert(isLoaded());
    setUniform(uniformLocation(name), value);
}

NVL_INLINE void QGLBaseShader::setUniform(const std::string& name, const Color& value)
{
    assert(isLoaded());
    setUniform(uniformLocation(name), value);
}

NVL_INLINE int QGLBaseShader::attributeLocation(const std::string& name) const
{
    assert(isLoaded());
    return vShaderProgram->attributeLocation(name.c_str());
}

NVL_INLINE void QGLBaseShader::setAttribute(const int& location, const double& value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLBaseShader::setAttribute(const int& location, const float& value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, static_cast<GLfloat>(value));
}

NVL_INLINE void QGLBaseShader::setAttribute(const int& location, const int& value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, static_cast<GLint>(value));
}

NVL_INLINE void QGLBaseShader::setAttribute(const int& location, const Color& value)
{
    assert(isLoaded());
    vShaderProgram->setAttributeValue(location, value.toQColor());
}

NVL_INLINE void QGLBaseShader::setAttribute(const std::string& name, const double& value)
{
    assert(isLoaded());
    setAttribute(attributeLocation(name), value);
}

NVL_INLINE void QGLBaseShader::setAttribute(const std::string& name, const float& value)
{
    assert(isLoaded());
    setAttribute(attributeLocation(name), value);
}

NVL_INLINE void QGLBaseShader::setAttribute(const std::string& name, const int& value)
{
    assert(isLoaded());
    setAttribute(attributeLocation(name), value);
}

NVL_INLINE void QGLBaseShader::setAttribute(const std::string& name, const Color& value)
{
    assert(isLoaded());
    setAttribute(attributeLocation(name), value);
}

NVL_INLINE bool QGLBaseShader::load(
        QGLContext* context,
        const std::string& vertexShader,
        const std::string& fragmentShader)
{
    createProgram(context);

    bool ret = true;
    ret &= vShaderProgram->addShaderFromSourceFile(QGLShader::Vertex, vertexShader.c_str());
    ret &= vShaderProgram->addShaderFromSourceFile(QGLShader::Fragment, fragmentShader.c_str());

    ret &= link();

    return ret;
}

bool NVL_INLINE QGLBaseShader::loadVertexShader(
    QGLContext* context,
    const std::string& vertexShader)
{
    createProgram(context);

    bool ret = true;
    ret &= vShaderProgram->addShaderFromSourceFile(QGLShader::Vertex, vertexShader.c_str());

    ret &= link();

    return ret;
}

bool NVL_INLINE QGLBaseShader::loadFragmentShader(
    QGLContext* context,
    const std::string& fragmentShader)
{
    createProgram(context);

    bool ret = true;
    ret &= vShaderProgram->addShaderFromSourceFile(QGLShader::Fragment, fragmentShader.c_str());

    ret &= link();

    return ret;
}

void NVL_INLINE QGLBaseShader::createProgram(
        QGLContext* context)
{
    unload();

    vShaderProgram = new QGLShaderProgram(context);
}

bool NVL_INLINE QGLBaseShader::link()
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
