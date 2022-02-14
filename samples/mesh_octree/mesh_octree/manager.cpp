#include "manager.h"

#include <nvl/models/algorithms/mesh_octree.h>
#include <nvl/viewer/gl/gl_primitives.h>

Manager::Manager(nvl::QCanvas* canvas, Drawer* drawer, nvl::PolygonMesh3d* mesh) :
    drawer(drawer),
    mesh(mesh)
{
    this->connect(canvas, &nvl::QCanvas::signal_canvasPicking, this, &Manager::slot_canvasPicking);

    this->vfAdj = nvl::meshVertexFaceAdjacencies(*mesh);
}

void Manager::slot_canvasPicking(
        const std::vector<nvl::Canvas::PickingData>& data,
        const nvl::Point2d& point2D,
        const bool& found,
        const nvl::Point3d& point3D,
        const nvl::Point3d& lineOrigin,
        const nvl::Vector3d& lineDirection)
{
    drawer->neighborNodes.clear();
    drawer->closestNode = nullptr;
    if (data.size() > 0) {
        nvl::Index selected = nvl::NULL_ID;
        for (nvl::Index i = 0; i < data.size(); ++i) {
            if (data[i].identifier == nvl::Canvas::PICKING_MESH_FACE) {
                selected = i;
                break;
            }
        }
        if (selected == nvl::NULL_ID) {
            drawer->picked = false;
            return;
        }

        drawer->picked = true;

        drawer->pickedPoint = point3D;
        drawer->closestPoint = nvl::meshVertexOctreeClosestPoint(*mesh, *(drawer->octree), point3D, vfAdj);

        const nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>::Node* closestNode = drawer->octree->findClosest(point3D);
        std::vector<const nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>::Node*> neighbors = closestNode->neighbors();

        drawer->closestNode = closestNode;

        drawer->neighborNodes.insert(neighbors.begin(), neighbors.end());
    }
    else {
        drawer->picked = false;
    }
}

Drawer::Drawer(nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>* octree) :
    octree(octree),
    picked(false)
{

}

void Drawer::draw() const
{
    if (octree != nullptr) {
        if (picked) {
            glDepthFunc(GL_ALWAYS);
            nvl::drawPoint(this->closestPoint, nvl::GREEN, 10);
            nvl::drawPoint(this->pickedPoint, nvl::RED, 5);
            glDepthFunc(GL_LEQUAL);
        }

        this->drawNode(octree->getRoot());
    }
}

nvl::Point3d Drawer::sceneCenter() const
{
    return octree->getCenter();
}

double Drawer::sceneRadius() const
{
    return octree->getRadius();
}

void Drawer::drawNode(const nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>::Node* node) const
{
    if (node == nullptr)
        return;

    //Clear subtrees
    for (const nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId>::Node* childNode : node->children) {
        drawNode(childNode);
    }

    if (node->isLeaf()) {
        double r = node->radius;
        nvl::Point3d v1 = node->center + nvl::Point3d(-r, -r, -r);
        nvl::Point3d v2 = node->center + nvl::Point3d(-r, -r, +r);
        nvl::Point3d v3 = node->center + nvl::Point3d(-r, +r, -r);
        nvl::Point3d v4 = node->center + nvl::Point3d(-r, +r, +r);
        nvl::Point3d v5 = node->center + nvl::Point3d(+r, -r, -r);
        nvl::Point3d v6 = node->center + nvl::Point3d(+r, -r, +r);
        nvl::Point3d v7 = node->center + nvl::Point3d(+r, +r, -r);
        nvl::Point3d v8 = node->center + nvl::Point3d(+r, +r, +r);

        glDepthFunc(GL_LESS);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glDepthRange(0.0, 1.0);
        glDepthFunc(GL_LEQUAL);

        if (closestNode == node) {
            glColor(nvl::GREEN);
            glLineWidth(4);
        }
        else if (neighborNodes.find(node) != neighborNodes.end()) {
            glColor(nvl::BLUE);
            glLineWidth(3);
        }
        else {
            glColor(nvl::BLACK);
            glLineWidth(1);
        }

        glBegin(GL_POLYGON);
        nvl::glVertex(v1);
        nvl::glVertex(v3);
        nvl::glVertex(v4);
        nvl::glVertex(v2);
        glEnd();
        glBegin(GL_POLYGON);
        nvl::glVertex(v5);
        nvl::glVertex(v7);
        nvl::glVertex(v8);
        nvl::glVertex(v6);
        glEnd();
        glBegin(GL_POLYGON);
        nvl::glVertex(v1);
        nvl::glVertex(v5);
        nvl::glVertex(v6);
        nvl::glVertex(v2);
        glEnd();
        glBegin(GL_POLYGON);
        nvl::glVertex(v3);
        nvl::glVertex(v7);
        nvl::glVertex(v8);
        nvl::glVertex(v4);
        glEnd();
    }
}
