#ifndef NVL_VIEWER_FACE_MESH_DRAWER_BASE_H
#define NVL_VIEWER_FACE_MESH_DRAWER_BASE_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/drawables/polyline_mesh_drawer_base.h>

#ifdef NVL_OPENGL_LOADED
#include <nvl/viewer/gl/gl_shader.h>
#endif

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
    enum TextureMode { TEXTURE_MODE_MODULATE, TEXTURE_MODE_REPLACE };

    FaceMeshDrawerBase();
    virtual ~FaceMeshDrawerBase() = default;

    virtual void update() = 0;

    bool faceVisible() const;
    virtual void setFaceVisible(const bool visible);

    bool wireframeVisible() const;
    virtual void setWireframeVisible(const bool visible);

    bool faceNormalVisible() const;
    virtual void setFaceNormalVisible(const bool visible);

    bool textureVisible() const;
    virtual void setTextureVisible(const bool visible);

    int wireframeSize() const;
    virtual void setWireframeSize(const int size);

    const Color& wireframeColor() const;
    virtual void setWireframeColor(const Color& color);

    int faceNormalSize() const;
    virtual void setFaceNormalSize(const int size);

    const Color& faceUniformColor() const;
    virtual void setFaceUniformColor(const Color& value);

    const FaceShadingMode& faceShadingMode() const;
    virtual void setFaceShadingMode(FaceShadingMode mode);

    const FaceColorMode& faceColorMode() const;
    virtual void setFaceColorMode(const FaceColorMode& mode);

    bool faceTransparency() const;
    virtual void setFaceTransparency(const bool value);

    bool faceLighting() const;
    virtual void setFaceLighting(const bool value);

    const TextureMode& textureMode() const;
    virtual void setTextureMode(const TextureMode& mode);

    const FaceShaderMode& faceShaderMode() const;
    virtual void setFaceShaderMode(const FaceShaderMode& mode);

#ifdef NVL_OPENGL_LOADED
    GLShader* faceShader() const;
    virtual void setFaceShader(GLShader* value);
#endif

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
    virtual bool hasFaceLighting() const = 0;
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

    bool vFaceTransparency;
    bool vFaceLighting;

    FaceShaderMode vFaceShaderMode;
#ifdef NVL_OPENGL_LOADED
    GLShader* vFaceShader;
#endif

    std::vector<double> vVertexValues;
    std::vector<double> vFaceValues;

    TextureMode vTextureMode;


};

}

#include "face_mesh_drawer_base.cpp"

#endif // NVL_VIEWER_FACE_MESH_DRAWER_BASE_H
