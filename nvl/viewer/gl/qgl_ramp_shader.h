#ifndef NVL_VIEWER_QGL_RAMP_SHADER_H
#define NVL_VIEWER_QGL_RAMP_SHADER_H

#include <nvl/nuvolib.h>

#ifdef NVL_QGLVIEWER_LOADED

#include <nvl/utilities/color.h>
#include <nvl/viewer/gl/qgl_base_shader.h>

namespace nvl {

class QGLRampShader : public QGLBaseShader
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
    void setVertexValues(std::vector<float>& vertexValues);

    float minValue() const;
    void setMinValue(const float minValue);

    float maxValue() const;
    void setMaxValue(const float maxValue);

    float alpha() const;
    void setAlpha(const float alpha);

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
    float vAlpha;
    Color vMinColor;
    Color vMidColor;
    Color vMaxColor;

    std::vector<float> vVertexValues;

};

}

#endif

#include "qgl_ramp_shader.cpp"

#endif // NVL_VIEWER_QGL_RAMP_SHADER_H
