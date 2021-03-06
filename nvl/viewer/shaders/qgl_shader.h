/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_QGL_SHADER_H
#define NVL_VIEWER_QGL_SHADER_H

#include <nvl/nuvolib.h>

#ifdef NVL_QGLVIEWER_LOADED

#include <nvl/viewer/shaders/gl_shader.h>
#include <nvl/utilities/color.h>

#include <QGLContext>
#include <QGLShaderProgram>

namespace nvl {

/**
 * @brief QGLViewer shader interface
 */
class QGLShader : public GLShader
{

public:

    QGLShader();
    virtual ~QGLShader();

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

    virtual void setUniform(const int& location, const Color& value);
    virtual void setUniform(const std::string& name, const Color& value);

    virtual void setAttribute(const int& location, const Color& value);
    virtual void setAttribute(const std::string& name, const Color& value);

protected:

    QGLShaderProgram* vShaderProgram;

    bool load(
        QGLContext* context,
        const std::string& vertexShader,
        const std::string& fragmentShader);
    bool loadVertexShader(
        QGLContext* context,
        const std::string& vertexShader);
    bool loadFragmentShader(
        QGLContext* context,
        const std::string& fragmentShader);

    void createProgram(
            QGLContext* context);
    bool link();

};

}

#endif

#include "qgl_shader.cpp"

#endif // NVL_VIEWER_QGL_SHADER_H
