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
    virtual void setPolylineVisible(bool visible);

    int polylineSize() const;
    virtual void setPolylineSize(int size);

    const Color& polylineUniformColor() const;
    virtual void setPolylineUniformColor(const Color& color);

    const PolylineShapeMode& polylineShapeMode() const;
    virtual void setPolylineShapeMode(PolylineShapeMode mode);

    const PolylineColorMode& polylineColorMode() const;
    virtual void setPolylineColorMode(PolylineColorMode mode);

    bool polylineTransparency() const;
    virtual void setPolylineTransparency(bool value);

    virtual bool hasPolylineColors() const = 0;
    virtual bool hasPolylineTransparency() const = 0;
    virtual bool hasVertexColors() const = 0;

private:

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
