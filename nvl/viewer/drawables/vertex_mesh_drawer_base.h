#ifndef NVL_VIEWER_VERTEX_DRAWER_BASE_H
#define NVL_VIEWER_VERTEX_DRAWER_BASE_H

#include <nvl/nuvolib.h>

#include <nvl/utilities/color.h>

#include <vector>

namespace nvl {

class VertexMeshDrawerBase
{

public:

    enum VertexShapeMode { VERTEX_SHAPE_POINT, VERTEX_SHAPE_DOT };
    enum VertexColorMode { VERTEX_COLOR_PER_VERTEX, VERTEX_COLOR_UNIFORM };

    VertexMeshDrawerBase();
    virtual ~VertexMeshDrawerBase() = default;

    virtual void update() = 0;

    bool vertexVisible() const;
    void setVertexVisible(bool visible);

    bool vertexNormalsVisible() const;
    void setVertexNormalVisible(bool visible);

    int vertexSize() const;
    void setVertexSize(int size);

    const Color& vertexUniformColor() const;
    void setVertexUniformColor(const Color& color);

    int vertexNormalSize() const;
    void setVertexNormalSize(int value);

    const VertexShapeMode& vertexShapeMode() const;
    void setVertexShapeMode(VertexShapeMode mode);

    const VertexColorMode& vertexColorMode() const;
    void setVertexColorMode(VertexColorMode mode);

    bool vertexTransparency() const;
    void setVertexTransparency(bool value);


protected:

    bool vVertexVisible;
    bool vVertexNormalsVisible;

    int vVertexSize;
    Color vVertexUniformColor;

    int vVertexNormalSize;

    VertexShapeMode vVertexShapeMode;
    VertexColorMode vVertexColorMode;

    bool vVertexTransparency;

};

}

#include "vertex_mesh_drawer_base.cpp"

#endif // NVL_VIEWER_VERTEX_DRAWER_BASE_H
