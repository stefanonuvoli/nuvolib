/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_GL_SHADER_H
#define NVL_VIEWER_GL_SHADER_H

#include <nvl/nuvolib.h>

#ifdef NVL_OPENGL_LOADED

#include <string>

#include <nvl/math/point.h>

namespace nvl {

class GLShader
{

public:

    GLShader();
    virtual ~GLShader() = default;

    virtual void bind() = 0;
    virtual void release() = 0;

    virtual void initGL() = 0;
    virtual void postGL() = 0;

    virtual void initFace(const Index& fId) = 0;
    virtual void postFace(const Index& fId) = 0;

    virtual void initVertex(const Index& vId) = 0;
    virtual void postVertex(const Index& vId) = 0;

    virtual void addVertex(const Index& vId, const Point3d& p) = 0;

    virtual int uniformLocation(const std::string& name) const = 0;
    virtual void setUniform(const int& location, const double& value) = 0;
    virtual void setUniform(const int& location, const float& value) = 0;
    virtual void setUniform(const int& location, const int& value) = 0;
    virtual void setUniform(const std::string& name, const double& value) = 0;
    virtual void setUniform(const std::string& name, const float& value) = 0;
    virtual void setUniform(const std::string& name, const int& value) = 0;

    virtual int attributeLocation(const std::string& name) const = 0;
    virtual void setAttribute(const int& location, const double& value) = 0;
    virtual void setAttribute(const int& location, const float& value) = 0;
    virtual void setAttribute(const int& location, const int& value) = 0;
    virtual void setAttribute(const std::string& name, const double& value) = 0;
    virtual void setAttribute(const std::string& name, const float& value) = 0;
    virtual void setAttribute(const std::string& name, const int& value) = 0;

};

}

#endif

#include "gl_shader.cpp"

#endif // NVL_VIEWER_GL_SHADER_H
