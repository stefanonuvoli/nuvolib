#ifndef NVL_VIEWER_VERTEX_MESH_DRAWER_H
#define NVL_VIEWER_VERTEX_MESH_DRAWER_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/drawables/vertex_mesh_drawer_base.h>
#include <nvl/viewer/drawables/mesh_drawer.h>

#include <nvl/utilities/color.h>

#include <vector>

namespace nvl {

template<class M>
class VertexMeshDrawer : public VertexMeshDrawerBase, public MeshDrawer<M>
{

public:

    VertexMeshDrawer();
    VertexMeshDrawer(M* mesh, const bool visible = true, const bool pickable = true);
    virtual ~VertexMeshDrawer() = default;

    virtual void draw() const override;
    virtual void drawWithNames(Canvas* canvas, const Index drawableId) const override;

    virtual void update() override;

    bool hasVertexNormals() const override;
    bool hasVertexColors() const override;
    bool hasVertexTransparency() const override;

    Point3d renderingVertex(const Index& id) const;
    Vector3d renderingVertexNormal(const Index& id) const;
    Color renderingVertexColor(const Index& id) const;

    virtual void resetRenderingData();
    void resetRenderingVertexData();

    void resetRenderingVertices();
    void resetRenderingVertexNormals();
    void resetRenderingVertexColors();
    void resetRenderingVertex(const Index& id);
    void resetRenderingVertexNormal(const Index& id);
    void resetRenderingVertexColor(const Index& id);

    void setRenderingVertices(const std::vector<double>& renderingVerticess);
    void setRenderingVertexNormals(const std::vector<double>& renderingVertexNormals);
    void setRenderingVertexColors(const std::vector<float>& renderingVertexColors);
    void setRenderingVertex(const Index& id, const Point3d& p);
    void setRenderingVertexNormal(const Index& id, const Vector3d& n);
    void setRenderingVertexColor(const Index& id, const Color& c);

protected:

    std::vector<Index> vVertexMap;

    std::vector<double> vRenderingVertices;
    std::vector<double> vRenderingVertexNormals;
    std::vector<float> vRenderingVertexColors;

    void drawVerticesPoint() const;
    void drawVerticesDot() const;
    void drawVertexNormals() const;
    void drawVerticesPointWithNames(Canvas* canvas, const Index drawableId) const;
    void drawVerticesDotWithNames(Canvas* canvas, const Index drawableId) const;

    double getVertexSize() const;
    double getVertexNormalRadius() const;
    double getVertexNormalLength() const;

    Color vDefaultVertexColor;
};

}

#include "vertex_mesh_drawer.cpp"

#endif // NVL_VIEWER_VERTEX_MESH_DRAWER_H
