/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
    virtual void setVertexVisible(bool visible);

    bool vertexNormalsVisible() const;
    virtual void setVertexNormalVisible(bool visible);

    int vertexSize() const;
    virtual void setVertexSize(int size);

    const Color& vertexUniformColor() const;
    virtual void setVertexUniformColor(const Color& color);

    int vertexNormalSize() const;
    virtual void setVertexNormalSize(int value);

    const VertexShapeMode& vertexShapeMode() const;
    virtual void setVertexShapeMode(VertexShapeMode mode);

    const VertexColorMode& vertexColorMode() const;
    virtual void setVertexColorMode(VertexColorMode mode);

    bool vertexTransparency() const;
    virtual void setVertexTransparency(bool value);

    virtual bool hasVertexColors() const = 0;
    virtual bool hasVertexNormals() const = 0;
    virtual bool hasVertexTransparency() const = 0;

private:

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
