#ifndef NVL_VIEWER_POLYLINE_MESH_DRAWER_BASE_H
#define NVL_VIEWER_POLYLINE_MESH_DRAWER_BASE_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/drawables/vertex_mesh_drawer_base.h>

#include <nvl/utilities/color.h>

namespace nvl {

class PolylineMeshDrawerBase
{

public:

    typedef typename VertexMeshDrawerBase::VertexShapeMode VertexShapeMode;
    typedef typename VertexMeshDrawerBase::VertexColorMode VertexColorMode;
    enum PolylineShapeMode { POLYLINE_SHAPE_LINE, POLYLINE_SHAPE_CYLINDER, POLYLINE_SHAPE_ARROW };
    enum PolylineColorMode { POLYLINE_COLOR_PER_POLYLINE, POLYLINE_COLOR_PER_VERTEX, POLYLINE_COLOR_UNIFORM };

    PolylineMeshDrawerBase();
    virtual ~PolylineMeshDrawerBase() = default;

    virtual void update() = 0;

    bool polylineVisible() const;
    void setPolylineVisible(bool visible);

    int polylineSize() const;
    void setPolylineSize(int size);

    const Color& polylineUniformColor() const;
    void setPolylineUniformColor(const Color& color);

    const PolylineShapeMode& polylineShapeMode() const;
    void setPolylineShapeMode(PolylineShapeMode mode);

    const PolylineColorMode& polylineColorMode() const;
    void setPolylineColorMode(PolylineColorMode mode);

    bool polylineTransparency() const;
    void setPolylineTransparency(bool value);

protected:

    bool vPolylineVisible;

    int vPolylineSize;
    Color vPolylineUniformColor;

    PolylineShapeMode vPolylineShapeMode;
    PolylineColorMode vPolylineColorMode;

    bool vPolylineTransparency;

};

}

#include "polyline_mesh_drawer_base.cpp"

#endif // NVL_VIEWER_POLYLINE_MESH_DRAWER_BASE_H
