#include "face_mesh_drawer_base.h"

#include <nvl/viewer/gl/draw_primitives.h>
#include <nvl/viewer/gl/draw_gl.h>

namespace nvl {

NVL_INLINE FaceMeshDrawerBase::FaceMeshDrawerBase() :
    vFaceVisible(true),
    vWireframeVisible(false),
    vFaceNormalVisible(false),
    vTextureVisible(true),
    vWireframeSize(1), vWireframeColor(0, 0, 0),
    vFaceNormalSize(5),
    vFaceUniformColor(0.7, 0.7, 0.7),
    vFaceShadingMode(FACE_SHADING_SMOOTH),
    vFaceColorMode(FACE_COLOR_PER_FACE),
    vFaceShaderMode(FACE_SHADER_NONE),
    vFaceShader(nullptr),
    vFaceTransparency(false)
{

}

NVL_INLINE bool FaceMeshDrawerBase::faceVisible() const
{
    return vFaceVisible;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceVisible(bool visible)
{
    vFaceVisible = visible;
}

NVL_INLINE bool FaceMeshDrawerBase::wireframeVisible() const
{
    return vWireframeVisible;
}

NVL_INLINE void FaceMeshDrawerBase::setWireframeVisible(bool visible)
{
    vWireframeVisible = visible;
}

NVL_INLINE bool FaceMeshDrawerBase::faceNormalVisible() const
{
    return vFaceNormalVisible;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceNormalVisible(bool visible)
{
    vFaceNormalVisible = visible;
}

NVL_INLINE bool FaceMeshDrawerBase::textureVisible() const
{
    return vTextureVisible;
}

NVL_INLINE void FaceMeshDrawerBase::setTextureVisible(bool visible)
{
    vTextureVisible = visible;
}

NVL_INLINE int FaceMeshDrawerBase::wireframeSize() const
{
    return vWireframeSize;
}

NVL_INLINE void FaceMeshDrawerBase::setWireframeSize(int size)
{
    vWireframeSize = size;
}

NVL_INLINE const Color& FaceMeshDrawerBase::wireframeColor() const
{
    return vWireframeColor;
}

NVL_INLINE void FaceMeshDrawerBase::setWireframeColor(const Color& color)
{
    vWireframeColor = color;
}

NVL_INLINE int FaceMeshDrawerBase::faceNormalSize() const
{
    return vFaceNormalSize;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceNormalSize(int size)
{
    vFaceNormalSize = size;
}

NVL_INLINE const Color& FaceMeshDrawerBase::faceUniformColor() const
{
    return vFaceUniformColor;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceUniformColor(const Color& color)
{
    vFaceUniformColor = color;
}

NVL_INLINE const typename FaceMeshDrawerBase::FaceShadingMode& FaceMeshDrawerBase::faceShadingMode() const
{
    return vFaceShadingMode;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceShadingMode(FaceShadingMode mode)
{
    vFaceShadingMode = mode;
}

NVL_INLINE const typename FaceMeshDrawerBase::FaceColorMode& FaceMeshDrawerBase::faceColorMode() const
{
    return vFaceColorMode;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceColorMode(FaceColorMode mode)
{
    vFaceColorMode = mode;
}

NVL_INLINE const typename FaceMeshDrawerBase::FaceShaderMode& FaceMeshDrawerBase::faceShaderMode() const
{
    return vFaceShaderMode;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceShaderMode(FaceShaderMode mode)
{
    vFaceShaderMode = mode;
}

NVL_INLINE GLShader* FaceMeshDrawerBase::faceShader() const
{
    return vFaceShader;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceShader(GLShader* value)
{
    vFaceShader = value;
}

NVL_INLINE std::vector<double>& FaceMeshDrawerBase::vertexValues()
{
    return vVertexValues;
}

NVL_INLINE void FaceMeshDrawerBase::setVertexValues(const std::vector<double>& value)
{
    vVertexValues = value;
}

NVL_INLINE void FaceMeshDrawerBase::clearVertexValues()
{
    vVertexValues.clear();
}

NVL_INLINE std::vector<double>& FaceMeshDrawerBase::faceValues()
{
    return vFaceValues;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceValues(const std::vector<double>& value)
{
    vFaceValues = value;
}

NVL_INLINE void FaceMeshDrawerBase::clearFaceValues()
{
    vFaceValues.clear();
}

NVL_INLINE bool FaceMeshDrawerBase::faceTransparency() const
{
    return vFaceTransparency;
}

NVL_INLINE void FaceMeshDrawerBase::setFaceTransparency(bool value)
{
    vFaceTransparency = value;
}

}
