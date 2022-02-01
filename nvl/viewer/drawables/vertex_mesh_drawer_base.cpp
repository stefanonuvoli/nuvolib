/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "vertex_mesh_drawer_base.h"

namespace nvl {

NVL_INLINE VertexMeshDrawerBase::VertexMeshDrawerBase() :
    vVertexVisible(true),
    vVertexNormalsVisible(false),
    vVertexSize(5),
    vVertexUniformColor(95, 95, 95),
    vVertexNormalSize(5),
    vVertexShapeMode(VERTEX_SHAPE_DOT),
    vVertexColorMode(VERTEX_COLOR_PER_VERTEX),
    vVertexTransparency(false)
{

}

NVL_INLINE bool VertexMeshDrawerBase::vertexVisible() const
{
    return vVertexVisible;
}

NVL_INLINE void VertexMeshDrawerBase::setVertexVisible(bool visible)
{
    vVertexVisible = visible;
}

NVL_INLINE bool VertexMeshDrawerBase::vertexNormalsVisible() const
{
    return vVertexNormalsVisible;
}

NVL_INLINE void VertexMeshDrawerBase::setVertexNormalVisible(bool visible)
{
    vVertexNormalsVisible = visible;
}

NVL_INLINE int VertexMeshDrawerBase::vertexSize() const
{
    return vVertexSize;
}

NVL_INLINE void VertexMeshDrawerBase::setVertexSize(int size)
{
    vVertexSize = size;
}

NVL_INLINE const Color& VertexMeshDrawerBase::vertexUniformColor() const
{
    return vVertexUniformColor;
}

NVL_INLINE void VertexMeshDrawerBase::setVertexUniformColor(const Color& color)
{
    vVertexUniformColor = color;
}

NVL_INLINE int VertexMeshDrawerBase::vertexNormalSize() const
{
    return vVertexNormalSize;
}

NVL_INLINE void VertexMeshDrawerBase::setVertexNormalSize(int value)
{
    vVertexNormalSize = value;
}

NVL_INLINE const typename VertexMeshDrawerBase::VertexShapeMode& VertexMeshDrawerBase::vertexShapeMode() const
{
    return vVertexShapeMode;
}

NVL_INLINE void VertexMeshDrawerBase::setVertexShapeMode(VertexShapeMode mode)
{
    vVertexShapeMode = mode;
}

NVL_INLINE const typename VertexMeshDrawerBase::VertexColorMode& VertexMeshDrawerBase::vertexColorMode() const
{
    return vVertexColorMode;
}

NVL_INLINE void VertexMeshDrawerBase::setVertexColorMode(VertexColorMode mode)
{
    vVertexColorMode = mode;
}

NVL_INLINE bool VertexMeshDrawerBase::vertexTransparency() const
{
    return vVertexTransparency;
}

NVL_INLINE void VertexMeshDrawerBase::setVertexTransparency(bool value)
{
    vVertexTransparency = value;
}

}
