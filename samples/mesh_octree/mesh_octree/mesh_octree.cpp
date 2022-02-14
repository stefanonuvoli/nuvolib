#include <QApplication>

#include <nvl/viewer/viewerwindow.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/widgets/drawable_widget.h>
#include <nvl/viewer/widgets/face_mesh_drawer_widget.h>
#include <nvl/viewer/widgets/vertex_mesh_drawer_widget.h>
#include <nvl/viewer/drawables/face_mesh_drawer.h>
#include <nvl/viewer/interfaces/drawable.h>

#include <nvl/math/transformations.h>

#include <nvl/models/mesh_3d.h>
#include <nvl/models/algorithms/mesh_transformations.h>
#include <nvl/models/algorithms/mesh_geometric_information.h>
#include <nvl/models/algorithms/mesh_octree.h>
#include <nvl/models/io/mesh_io.h>

#include <nvl/structures/trees/octree.h>

#include "manager.h"

int main(int argc, char *argv[]) {
    typedef nvl::PolygonMesh3d Mesh;
    typedef nvl::FaceMeshDrawer<Mesh> FaceMeshDrawer;

    QApplication app(argc, argv);

    nvl::ViewerWindow viewer;
    viewer.setWindowTitle("Mesh octree sample");
    viewer.update();

    viewer.showRightDock();

    //Add canvas
    nvl::QGLViewerCanvas canvas(&viewer);
    viewer.addCanvas(&canvas);

    //Right dock
    nvl::DrawableListWidget drawableListWidget(&canvas);
    viewer.addToRightDock(&drawableListWidget);
    nvl::DrawableWidget drawableWidget(&canvas, &drawableListWidget);
    viewer.addToRightDock(&drawableWidget);
    nvl::FaceMeshDrawerWidget faceMeshDrawerWidget(&canvas, &drawableListWidget);
    viewer.addToRightDock(&faceMeshDrawerWidget);
    nvl::VertexMeshDrawerWidget vertexMeshDrawerWidget(&canvas, &drawableListWidget);
    viewer.addToRightDock(&vertexMeshDrawerWidget);

    //Meshes
    Mesh mesh;

    //Load mesh
    nvl::meshLoadFromFile("../../data/bunny_2000.obj", mesh);

    //Translate mesh to the center
    nvl::AlignedBox3d bbox = nvl::meshBoundingBox(mesh);
    nvl::Vector3d center = -bbox.center();
    nvl::meshApplyTransformation(mesh, nvl::Translation3d(center));

    //Update the vertex and face normals
    mesh.computeFaceNormals();
    mesh.computeVertexNormals();
    mesh.enablePolylineColors();

    //Create octree for the mesh
    nvl::Octree<nvl::PolygonMesh3d::Point, nvl::PolygonMesh3d::VertexId> octree = nvl::meshVertexOctree(mesh);

    //Initialize the drawers
    FaceMeshDrawer meshDrawer(&mesh);
    Drawer drawer(&octree);

    //Add to the canvas
    viewer.canvas()->addDrawable(&meshDrawer);
    viewer.canvas()->addDrawable(&drawer);

    //Call of fit scene
    viewer.canvas()->fitScene();

    viewer.showMaximized(); //Show the viewer

    Manager manager(&canvas, &drawer, &mesh);

    return app.exec();
}
