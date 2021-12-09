#ifndef NVL_VIEWER_FACE_MESH_DRAWER_BASE_H
#define NVL_VIEWER_FACE_MESH_DRAWER_BASE_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/drawables/polyline_mesh_drawer_base.h>
#include <nvl/viewer/gl/gl_shader.h>

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
    virtual void setFaceVisible(bool visible);

    bool wireframeVisible() const;
    virtual void setWireframeVisible(bool visible);

    bool faceNormalVisible() const;
    virtual void setFaceNormalVisible(bool visible);

    bool textureVisible() const;
    virtual void setTextureVisible(bool visible);

    int wireframeSize() const;
    virtual void setWireframeSize(int size);

    const Color& wireframeColor() const;
    virtual void setWireframeColor(const Color& color);

    int faceNormalSize() const;
    virtual void setFaceNormalSize(int size);

    const Color& faceUniformColor() const;
    virtual void setFaceUniformColor(const Color& value);

    const FaceShadingMode& faceShadingMode() const;
    virtual void setFaceShadingMode(FaceShadingMode mode);

    const FaceColorMode& faceColorMode() const;
    virtual void setFaceColorMode(FaceColorMode mode);

    const FaceShaderMode& faceShaderMode() const;
    virtual void setFaceShaderMode(const FaceShaderMode& mode);

    GLShader* faceShader() const;
    virtual void setFaceShader(GLShader* value);

    bool faceTransparency() const;
    virtual void setFaceTransparency(bool value);

    std::vector<double>& vertexValues();
    const std::vector<double>& vertexValues() const;
    virtual void setVertexValues(const std::vector<double>& value);
    virtual void clearVertexValues();

    std::vector<double>& faceValues();
    const std::vector<double>& faceValues() const;
    virtual void setFaceValues(const std::vector<double>& value);
    virtual void clearFaceValues();

    virtual bool hasFaceNormals() const = 0;
    virtual bool hasFaceColors() const = 0;
    virtual bool hasTextures() const = 0;
    virtual bool hasFaceTransparency() const = 0;
    virtual bool hasSmoothShading() const = 0;
    virtual bool hasFlatShading() const = 0;
    virtual bool hasVertexColors() const = 0;


private:

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
