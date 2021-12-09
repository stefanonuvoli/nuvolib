#ifndef NVL_VIEWER_GL_SHADER_H
#define NVL_VIEWER_GL_SHADER_H

#include <nvl/nuvolib.h>

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

    virtual void setUniform(int location, double value) = 0;
    virtual void setAttribute(int location, double value) = 0;
    virtual void setUniform(const std::string& name, double value) = 0;
    virtual void setAttribute(const std::string& name, double value) = 0;

    virtual int attributeLocation(const std::string& name) const = 0;
    virtual int uniformLocation(const std::string& name) const = 0;

};

}

#include "gl_shader.cpp"

#endif // NVL_VIEWER_GL_SHADER_H
