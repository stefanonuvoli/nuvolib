#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

#include <nvl/viewer/widgets/qcanvas.h>

#include <nvl/structures/trees/octree.h>

#include <nvl/models/mesh_3d.h>

class Drawer : public nvl::Drawable
{

public:

    Drawer(nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>* octree);
    virtual ~Drawer() = default;

    void draw() const;
    nvl::Point3d sceneCenter() const;
    double sceneRadius() const;

    nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>* octree;

    nvl::Point3d pickedPoint;
    nvl::Point3d closestPoint;
    const nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>::Node* closestNode;
    std::unordered_set<const nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>::Node*> neighborNodes;
    bool picked;

private:

    void drawNode(const nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>::Node* node) const;
};

class Manager : public QObject
{
    Q_OBJECT

public:

    Manager(nvl::QCanvas* canvas, Drawer* drawer, nvl::PolygonMesh3d* mesh);
    virtual ~Manager() = default;

    Drawer* drawer;
    nvl::PolygonMesh3d* mesh;

private:

    std::vector<std::vector<nvl::PolygonMesh3d::FaceId>> vfAdj;

public Q_SLOTS:

    void slot_canvasPicking(
            const std::vector<nvl::Canvas::PickingData>& data,
            const nvl::Point2d& point2D,
            const bool& found,
            const nvl::Point3d& point3D,
            const nvl::Point3d& lineOrigin,
            const nvl::Vector3d& lineDirection);

};

#endif // MANAGER_H
