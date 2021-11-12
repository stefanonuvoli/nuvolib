#ifndef NVL_VIEWER_FACE_MESH_DRAWER_BASE_H
#define NVL_VIEWER_FACE_MESH_DRAWER_BASE_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/drawables/polyline_mesh_drawer_base.h>
#include <nvl/viewer/gl/glshader.h>

#include <nvl/utilities/color.h>

#include <vector>

namespace nvl {

class FaceMeshDrawerBase
{

public:

    typedef typename PolylineMeshDrawerBase::VertexShapeMode VertexShapeMode;
    typedef typename PolylineMeshDrawerBase::VertexColorMode VertexColorMode;
    typedef typename PolylineMeshDrawerBase::PolylineShapeMode PolylineShapeMode;
    typedef typename PolylineMeshDrawerBase::PolylineColorMode PolylineColorMode;
    enum FaceShadingMode { FACE_SHADING_SMOOTH, FACE_SHADING_FLAT };
    enum FaceColorMode { FACE_COLOR_PER_FACE, FACE_COLOR_PER_VERTEX, FACE_COLOR_UNIFORM };
    enum FaceShaderMode { FACE_SHADER_VERTEX_VALUE, FACE_SHADER_NONE };

    FaceMeshDrawerBase();
    virtual ~FaceMeshDrawerBase() = default;

    virtual void update() = 0;

    bool faceVisible() const;
    void setFaceVisible(bool visible);

    bool wireframeVisible() const;
    void setWireframeVisible(bool visible);

    bool faceNormalVisible() const;
    void setFaceNormalVisible(bool visible);

    bool textureVisible() const;
    void setTextureVisible(bool visible);

    int wireframeSize() const;
    void setWireframeSize(int size);
    const Color& wireframeColor() const;
    void setWireframeColor(const Color& color);

    int faceNormalSize() const;
    void setFaceNormalSize(int size);

    const Color& faceUniformColor() const;
    void setFaceUniformColor(const Color& value);

    const FaceShadingMode& faceShadingMode() const;
    void setFaceShadingMode(FaceShadingMode mode);

    const FaceColorMode& faceColorMode() const;
    void setFaceColorMode(FaceColorMode mode);

    const FaceShaderMode& faceShaderMode() const;
    void setFaceShaderMode(FaceShaderMode mode);

    GLShader* faceShader() const;
    void setFaceShader(GLShader* value);

    std::vector<double>& vertexValues();
    void setVertexValues(const std::vector<double>& value);
    void clearVertexValues();

    std::vector<double>& faceValues();
    void setFaceValues(const std::vector<double>& value);
    void clearFaceValues();

    bool faceTransparency() const;
    void setFaceTransparency(bool value);


protected:

    bool vFaceVisible;
    bool vWireframeVisible;
    bool vFaceNormalVisible;
    bool vTextureVisible;

    int vWireframeSize;
    Color vWireframeColor;

    int vFaceNormalSize;

    Color vFaceUniformColor;

    FaceShadingMode vFaceShadingMode;
    FaceColorMode vFaceColorMode;

    FaceShaderMode vFaceShaderMode;
    GLShader* vFaceShader;

    std::vector<double> vVertexValues;
    std::vector<double> vFaceValues;

    bool vFaceTransparency;

};

}

#include "face_mesh_drawer_base.cpp"

#endif // NVL_VIEWER_FACE_MESH_DRAWER_BASE_H
