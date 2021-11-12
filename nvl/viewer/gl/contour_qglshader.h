#ifndef NVL_VIEWER_CONTOUR_QGLSHADER_H
#define NVL_VIEWER_CONTOUR_QGLSHADER_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/gl/glshader.h>

#include <QGLContext>
#include <QGLShaderProgram>

namespace nvl {

class ContourQGLShader : public GLShader
{

public:

    ContourQGLShader();
    virtual ~ContourQGLShader();

    bool load(QGLContext* context = nullptr);
    void unload();
    bool isLoaded() const;

    void bind() override;
    void release() override;

    void initGL() override;
    void postGL() override;

    void initFace(const Index& fId) override;
    void postFace(const Index& fId) override;

    void initVertex(const Index& vId) override;
    void postVertex(const Index& vId) override;

    void addVertex(const Index& vId, const Point3d& p) override;

    void setUniform(int location, double value) override;
    void setAttribute(int location, double value) override;
    virtual void setUniform(const std::string& name, double value) override;
    virtual void setAttribute(const std::string& name, double value) override;

    int attributeLocation(const std::string& name) const override;
    int uniformLocation(const std::string& name) const override;

protected:

    QGLShaderProgram* vShaderProgram;

};

}

#include "contour_qglshader.cpp"

#endif // NVL_VIEWER_CONTOUR_QGLSHADER_H
