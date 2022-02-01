/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "qgl_ramp_shader.h"

#ifdef NVL_QGLVIEWER_LOADED

#include <nvl/viewer/gl/gl_draw.h>

namespace nvl {

NVL_INLINE QGLRampShader::QGLRampShader() :
    vShaderProgram(nullptr),
    vMinValue(0.0f),
    vMaxValue(1.0f),
    vMinColor(1.0, 0.2, 0.2, 0.8),
    vMidColor(0.2, 1.0, 0.2, 0.8),
    vMaxColor(0.2, 0.2, 1.0, 0.8)
{

}

NVL_INLINE bool QGLRampShader::load(QGLContext* context)
{
    return QGLShader::load(context, ":/shaders/ramp.vert", ":/shaders/ramp.frag");
}

NVL_INLINE void QGLRampShader::initGL()
{
    this->setUniform("value_min", this->vMinValue);
    this->setUniform("value_max", this->vMaxValue);
    this->setUniform("col_min", this->vMinColor);
    this->setUniform("col_mid", this->vMidColor);
    this->setUniform("col_max", this->vMaxColor);

    vertexValueAttribute = this->attributeLocation("value");

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthRange(0.0001, 1.0);
    glDepthFunc(GL_LEQUAL);
}

NVL_INLINE void QGLRampShader::postGL()
{
    glDisable(GL_BLEND);
    glDepthRange(0.0, 1.0);
}

NVL_INLINE void QGLRampShader::initFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glBegin(GL_POLYGON);
}

NVL_INLINE void QGLRampShader::postFace(const Index& fId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(fId);
    glEnd();
}

NVL_INLINE void QGLRampShader::initVertex(const Index& vId)
{
    this->setAttribute(vertexValueAttribute, this->vVertexValues[vId]);
}

NVL_INLINE void QGLRampShader::postVertex(const Index& vId)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
}

NVL_INLINE void QGLRampShader::addVertex(const Index& vId, const Point3d& p)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(vId);
    glVertex(p);
}

NVL_INLINE std::vector<float>& QGLRampShader::vertexValues()
{
    return this->vVertexValues;
}

NVL_INLINE const std::vector<float>& QGLRampShader::vertexValues() const
{
    return this->vVertexValues;
}

NVL_INLINE void QGLRampShader::setVertexValues(std::vector<float>& vertexValues)
{
    this->vVertexValues = vertexValues;
}

NVL_INLINE float QGLRampShader::minValue() const
{
    return this->vMinValue;
}

NVL_INLINE void QGLRampShader::setMinValue(const float minValue)
{
    this->vMinValue = minValue;
}

NVL_INLINE float QGLRampShader::maxValue() const
{
    return this->vMaxValue;
}

NVL_INLINE void QGLRampShader::setMaxValue(const float maxValue)
{
    this->vMaxValue = maxValue;
}

NVL_INLINE const Color& QGLRampShader::minColor() const
{
    return vMinColor;
}

NVL_INLINE void QGLRampShader::setMinColor(const Color& minColor)
{
    this->vMinColor = minColor;
}

NVL_INLINE const Color& QGLRampShader::midColor() const
{
    return this->vMidColor;
}

NVL_INLINE void QGLRampShader::setMidColor(const Color& midColor)
{
    this->vMidColor = midColor;
}

NVL_INLINE const Color& QGLRampShader::maxColor() const
{
    return this->vMaxColor;
}

NVL_INLINE void QGLRampShader::setMaxColor(const Color& maxColor)
{
    this->vMaxColor = maxColor;
}

}

#endif
