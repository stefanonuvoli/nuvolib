/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_QGL_RAMP_SHADER_H
#define NVL_VIEWER_QGL_RAMP_SHADER_H

#include <nvl/nuvolib.h>

#ifdef NVL_QGLVIEWER_LOADED

#include <nvl/utilities/color.h>
#include <nvl/viewer/shaders/qgl_shader.h>

namespace nvl {

class QGLRampShader : public QGLShader
{

public:

    QGLRampShader();
    virtual ~QGLRampShader() = default;

    bool load(QGLContext* context);

    void initGL() override;
    void postGL() override;

    void initFace(const Index& fId) override;
    void postFace(const Index& fId) override;

    void initVertex(const Index& vId) override;
    void postVertex(const Index& vId) override;

    void addVertex(const Index& vId, const Point3d& p) override;

    std::vector<float>& vertexValues();
    const std::vector<float>& vertexValues() const;
    void setVertexValues(const std::vector<float>& vertexValues);

    float minValue() const;
    void setMinValue(const float minValue);

    float maxValue() const;
    void setMaxValue(const float maxValue);

    const Color& minColor() const;
    void setMinColor(const Color& minColor);

    const Color& midColor() const;
    void setMidColor(const Color& midColor);

    const Color& maxColor() const;
    void setMaxColor(const Color& maxColor);

protected:

    QGLShaderProgram* vShaderProgram;

    int vertexValueAttribute;

    float vMinValue;
    float vMaxValue;
    Color vMinColor;
    Color vMidColor;
    Color vMaxColor;

    std::vector<float> vVertexValues;

};

}

#endif

#include "qgl_ramp_shader.cpp"

#endif // NVL_VIEWER_QGL_RAMP_SHADER_H
