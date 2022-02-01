/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "polyline_mesh_drawer_base.h"

namespace nvl {

NVL_INLINE PolylineMeshDrawerBase::PolylineMeshDrawerBase() :
    vPolylineVisible(true),
    vPolylineSize(5),
    vPolylineUniformColor(0.7, 0.7, 0.7),
    vPolylineShapeMode(POLYLINE_SHAPE_CYLINDER),
    vPolylineColorMode(POLYLINE_COLOR_PER_POLYLINE),
    vPolylineTransparency(false)
{

}

NVL_INLINE bool PolylineMeshDrawerBase::polylineVisible() const
{
    return vPolylineVisible;
}

NVL_INLINE void PolylineMeshDrawerBase::setPolylineVisible(bool visible)
{
    vPolylineVisible = visible;
}

NVL_INLINE int PolylineMeshDrawerBase::polylineSize() const
{
    return vPolylineSize;
}

NVL_INLINE void PolylineMeshDrawerBase::setPolylineSize(int size)
{
    vPolylineSize = size;
}

NVL_INLINE const Color& PolylineMeshDrawerBase::polylineUniformColor() const
{
    return vPolylineUniformColor;
}

NVL_INLINE void PolylineMeshDrawerBase::setPolylineUniformColor(const Color& color)
{
    vPolylineUniformColor = color;
}

NVL_INLINE const typename PolylineMeshDrawerBase::PolylineShapeMode& PolylineMeshDrawerBase::polylineShapeMode() const
{
    return vPolylineShapeMode;
}

NVL_INLINE void PolylineMeshDrawerBase::setPolylineShapeMode(PolylineShapeMode mode)
{
    vPolylineShapeMode = mode;
}

NVL_INLINE const typename PolylineMeshDrawerBase::PolylineColorMode& PolylineMeshDrawerBase::polylineColorMode() const
{
    return vPolylineColorMode;
}

NVL_INLINE void PolylineMeshDrawerBase::setPolylineColorMode(PolylineColorMode mode)
{
    vPolylineColorMode = mode;
}

NVL_INLINE bool PolylineMeshDrawerBase::polylineTransparency() const
{
    return vPolylineTransparency;
}

NVL_INLINE void PolylineMeshDrawerBase::setPolylineTransparency(bool value)
{
    vPolylineTransparency = value;
}

}
