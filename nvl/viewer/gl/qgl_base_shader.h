#ifndef NVL_VIEWER_QGL_BASE_SHADER_H
#define NVL_VIEWER_QGL_BASE_SHADER_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/gl/gl_shader.h>

#include <QGLContext>
#include <QGLShaderProgram>

namespace nvl {

class QGLBaseShader : public GLShader
{

public:

    QGLBaseShader();
    virtual ~QGLBaseShader();

    virtual void unload();
    virtual bool isLoaded() const;

    virtual void bind() override;
    virtual void release() override;

    virtual int uniformLocation(const std::string& name) const override;
    virtual void setUniform(const int& location, const double& value) override;
    virtual void setUniform(const int& location, const float& value) override;
    virtual void setUniform(const int& location, const int& value) override;
    virtual void setUniform(const std::string& name, const double& value) override;
    virtual void setUniform(const std::string& name, const float& value) override;
    virtual void setUniform(const std::string& name, const int& value) override;

    virtual int attributeLocation(const std::string& name) const override;
    virtual void setAttribute(const int& location, const double& value) override;
    virtual void setAttribute(const int& location, const float& value) override;
    virtual void setAttribute(const int& location, const int& value) override;
    virtual void setAttribute(const std::string& name, const double& value) override;
    virtual void setAttribute(const std::string& name, const float& value) override;
    virtual void setAttribute(const std::string& name, const int& value) override;

protected:

    QGLShaderProgram* vShaderProgram;

    bool load(
        QGLContext* context,
        const std::string& vertexShader,
        const std::string& fragmentShader);

};

}

#include "qgl_base_shader.cpp"

#endif // NVL_VIEWER_QGL_BASE_SHADER_H
