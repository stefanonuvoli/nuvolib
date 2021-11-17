#ifndef NVL_VIEWER_POLYLINE_MESH_DRAWER_H
#define NVL_VIEWER_POLYLINE_MESH_DRAWER_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/drawables/vertex_mesh_drawer.h>
#include <nvl/viewer/drawables/polyline_mesh_drawer_base.h>

#include <nvl/utilities/color.h>

#include <vector>

namespace nvl {

template<class M>
class PolylineMeshDrawer : public PolylineMeshDrawerBase, public VertexMeshDrawer<M>
{

public:

    PolylineMeshDrawer();
    PolylineMeshDrawer(M* mesh, const bool visible = true, const bool pickable = true);
    virtual ~PolylineMeshDrawer() = default;

    virtual void draw() const override;
    virtual void drawWithNames(Canvas* canvas, const Index drawableId) const override;

    virtual void update() override;

    bool hasPolylineColors() const override;
    bool hasPolylineTransparency() const override;
    bool hasVertexColors() const override;

    const std::vector<unsigned int>& renderingPolyline(const Index& id) const;
    Color renderingPolylineColor(const Index& id) const;

    virtual void resetRenderingData();
    void resetRenderingPolylineData();

    void resetRenderingPolylines();
    void resetRenderingPolylineColors();
    void resetRenderingPolyline(const Index& id);
    void resetRenderingPolylineColor(const Index& id);

    void setRenderingPolylines(const std::vector<std::vector<unsigned int>>& renderingPolyliness);
    void setRenderingPolylineColors(const std::vector<float>& renderingPolylineColors);
    void setRenderingPolyline(const Index& id, const std::vector<unsigned int>& p);
    void setRenderingPolylineColor(const Index& id, const Color& c);

protected:

    std::vector<Index> vPolylineMap;

    std::vector<std::vector<unsigned int>> vRenderingPolylines;
    std::vector<float> vRenderingPolylineColors;

    void drawPolylinesLines() const;
    void drawPolylinesCylinder() const;
    void drawPolylinesArrow() const;
    void drawPolylinesLinesWithNames(Canvas* canvas, const Index drawableId) const;
    void drawPolylinesCylinderWithNames(Canvas* canvas, const Index drawableId) const;
    void drawPolylinesArrowWithNames(Canvas* canvas, const Index drawableId) const;

    double getPolylineSize() const;
    double getPolylineRadius() const;

    Color vDefaultPolylineColor;
};

}

#include "polyline_mesh_drawer.cpp"

#endif // NVL_VIEWER_POLYLINE_MESH_DRAWER_H
