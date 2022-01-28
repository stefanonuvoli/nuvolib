#include "qgl_contour_shader.h"

#ifdef NVL_QGLVIEWER_LOADED

#include <nvl/viewer/gl/gl_draw.h>

namespace nvl {

NVL_INLINE QGLContourShader::QGLContourShader() :
    vShaderProgram(nullptr),
    vMinValue(0.0f),
    vMaxValue(1.0f),
    vAlpha(0.8f),
    vStripeNumber(10.0f),
    vStripeWidth(0.6f),
    vMinColor(1.0, 0.2, 0.2),
    vMaxColor(0.2, 0.2, 1.0)
{

}

NVL_INLINE bool QGLContourShader::load(QGLContext* context)
{
    return QGLBaseShader::load(context, ":/shaders/contour.vert", ":/shaders/contour.frag");
}

NVL_INLINE void QGLContourShader::initGL()
{
    setUniform("value_min", this->vMinValue);
    setUniform("value_max", this->vMaxValue);
    setUniform("alpha", this->vAlpha);
    setUniform("stripe_num", this->vStripeNumber);
    setUniform("stripe_width", this->vStripeWidth);
    setUniform("col_min", this->vMinColor);
    setUniform("col_max", this->vMaxColor);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthRange(0.0001, 1.0);
    glDepthFunc(GL_LEQUAL);
}

NVL_INLINE void QGLContourShader::postGL()
{
    glDisable(GL_BLEND);
    glDepthRange(0.0, 1.0);
}

NVL_INLINE void QGLContourShader::initFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glBegin(GL_POLYGON);
}

NVL_INLINE void QGLContourShader::postFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glEnd();
}

NVL_INLINE void QGLContourShader::initVertex(const Index& vId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
}

NVL_INLINE void QGLContourShader::postVertex(const Index& vId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
}

NVL_INLINE void QGLContourShader::addVertex(const Index& vId, const Point3d& p)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
    glVertex(p);
}

NVL_INLINE std::vector<float>& QGLContourShader::vertexValues()
{
    return this->vVertexValues;
}

NVL_INLINE const std::vector<float>& QGLContourShader::vertexValues() const
{
    return this->vVertexValues;
}

NVL_INLINE void QGLContourShader::setVertexValues(std::vector<float>& vertexValues)
{
    this->vVertexValues = vertexValues;
}

NVL_INLINE float QGLContourShader::minValue() const
{
    return this->vMinValue;
}

NVL_INLINE void QGLContourShader::setMinValue(const float minValue)
{
    this->vMinValue = minValue;
}

NVL_INLINE float QGLContourShader::maxValue() const
{
    return this->vMaxValue;
}

NVL_INLINE void QGLContourShader::setMaxValue(const float maxValue)
{
    this->vMaxValue = maxValue;
}

NVL_INLINE float QGLContourShader::alpha() const
{
    return this->vAlpha;
}

NVL_INLINE void QGLContourShader::setAlpha(const float newAlpha)
{
    this->vAlpha = newAlpha;
}

NVL_INLINE float QGLContourShader::stripeNumber() const
{
    return this->vStripeNumber;
}

NVL_INLINE void QGLContourShader::setStripeNumber(const float stripeNumber)
{
    this->vStripeNumber = stripeNumber;
}

NVL_INLINE float QGLContourShader::stripeWidth() const
{
    return this->vStripeWidth;
}

NVL_INLINE void QGLContourShader::setStripeWidth(const float stripeWidth)
{
    this->vStripeWidth = stripeWidth;
}

NVL_INLINE const Color& QGLContourShader::minColor() const
{
    return vMinColor;
}

NVL_INLINE void QGLContourShader::setMinColor(const Color& minColor)
{
    this->vMinColor = minColor;
}

NVL_INLINE const Color& QGLContourShader::maxColor() const
{
    return this->vMaxColor;
}

NVL_INLINE void QGLContourShader::setMaxColor(const Color& maxColor)
{
    this->vMaxColor = maxColor;
}

}

#endif
