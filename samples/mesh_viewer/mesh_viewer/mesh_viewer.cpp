#include <QApplication>

#include <nvl/viewer/viewerwindow.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/widgets/drawable_widget.h>
#include <nvl/viewer/widgets/face_mesh_drawer_widget.h>
#include <nvl/viewer/widgets/polyline_mesh_drawer_widget.h>
#include <nvl/viewer/widgets/vertex_mesh_drawer_widget.h>
#include <nvl/viewer/drawables/face_mesh_drawer.h>
#include <nvl/viewer/interfaces/drawable.h>

#include <nvl/math/transformations.h>

#include <nvl/models/mesh_3d.h>
#include <nvl/models/algorithms/mesh_normals.h>
#include <nvl/models/algorithms/mesh_transformations.h>
#include <nvl/models/algorithms/mesh_geometric_information.h>
#include <nvl/models/io/mesh_io.h>

int main(int argc, char *argv[]) {
    typedef nvl::PolygonMesh3d Mesh;
    typedef nvl::VertexMeshDrawer<Mesh> VertexMeshDrawer;
    typedef nvl::PolylineMeshDrawer<Mesh> PolylineMeshDrawer;
    typedef nvl::FaceMeshDrawer<Mesh> FaceMeshDrawer;

    QApplication app(argc, argv);

    nvl::ViewerWindow viewer;
    viewer.setWindowTitle("Mesh viewer sample");
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
    nvl::PolylineMeshDrawerWidget polylineMeshDrawerWidget(&canvas, &drawableListWidget);
    viewer.addToRightDock(&polylineMeshDrawerWidget);
    nvl::VertexMeshDrawerWidget vertexMeshDrawerWidget(&canvas, &drawableListWidget);
    viewer.addToRightDock(&vertexMeshDrawerWidget);

    //Meshes
    Mesh mesh1, mesh2, mesh3, mesh4, mesh5;

    //Load mesh
    nvl::meshLoadFromFile("../../data/cube.obj", mesh1);

    //Translate mesh to the center
    nvl::AlignedBox3d bbox = nvl::meshBoundingBox(mesh1);
    nvl::Vector3d center = -bbox.center();
    nvl::meshApplyTransformation(mesh1, nvl::Translation3d(center));

    //Update the vertex and face normals
    mesh1.computeFaceNormals();
    mesh1.computeVertexNormals();
    mesh1.enablePolylineColors();

    //Copy the same mesh
    mesh5 = mesh4 = mesh3 = mesh2 = mesh1;

    //We apply a transformation to the first three meshes
    nvl::meshApplyTransformation(mesh1, nvl::Scaling3d(0.7, 0.7, 0.7));
    nvl::meshApplyTransformation(mesh2, nvl::Translation3d(5, 0, 0));
    nvl::meshApplyTransformation(mesh3, nvl::Translation3d(-5, 0, 0));

    //Initialize the drawers
    FaceMeshDrawer drawer1(&mesh1);
    FaceMeshDrawer drawer2(&mesh2);
    FaceMeshDrawer drawer3(&mesh3);
    VertexMeshDrawer drawer4(&mesh4);
    PolylineMeshDrawer drawer5(&mesh5);

    //Add to the canvas
    viewer.canvas()->addDrawable(&drawer1);
    viewer.canvas()->addDrawable(&drawer2);
    viewer.canvas()->addDrawable(&drawer3);
    viewer.canvas()->addDrawable(&drawer4);
    viewer.canvas()->addDrawable(&drawer5);

    //Drawer 1 settings
    drawer1.setFaceVisible(false);
    drawer1.setWireframeVisible(true);

    drawer1.setVertexVisible(true);
    drawer1.setVertexShapeMode(FaceMeshDrawer::VERTEX_SHAPE_DOT);
    drawer1.setVertexColorMode(FaceMeshDrawer::VERTEX_COLOR_UNIFORM);
    drawer1.setVertexUniformColor(nvl::RED);

    drawer1.setPolylineVisible(true);
    drawer1.setPolylineShapeMode(FaceMeshDrawer::POLYLINE_SHAPE_CYLINDER);
    drawer1.setPolylineColorMode(FaceMeshDrawer::POLYLINE_COLOR_PER_VERTEX);
    drawer1.setPolylineSize(10);

    drawer1.setFaceColorMode(FaceMeshDrawer::FACE_COLOR_UNIFORM);
    drawer1.setFaceUniformColor(nvl::RED);
    drawer1.setFaceNormalVisible(true);

    //Drawer 2 settings
    drawer2.setFaceShadingMode(FaceMeshDrawer::FACE_SHADING_SMOOTH);
    drawer2.setFaceColorMode(FaceMeshDrawer::FACE_COLOR_PER_FACE);

    //Drawer 3 settings
    drawer3.setFaceShadingMode(FaceMeshDrawer::FACE_SHADING_FLAT);
    drawer3.setFaceColorMode(FaceMeshDrawer::FACE_COLOR_PER_FACE);

    //Drawer 4 settings
    drawer4.setVertexNormalVisible(true);
    drawer4.setVertexShapeMode(VertexMeshDrawer::VERTEX_SHAPE_POINT);
    drawer4.setVertexColorMode(VertexMeshDrawer::VERTEX_COLOR_PER_VERTEX);

    //Drawer 5 settings
    drawer5.setPolylineShapeMode(PolylineMeshDrawer::POLYLINE_SHAPE_ARROW);
    drawer5.setPolylineColorMode(PolylineMeshDrawer::POLYLINE_COLOR_UNIFORM);
    drawer5.setPolylineSize(10);
    drawer5.setPolylineUniformColor(nvl::BLUE);

    //Update needed just for the drawers in order to draw the transformed mesh
    nvl::meshApplyTransformation(mesh4, nvl::Translation3d(0, 5, 0));
    nvl::meshApplyTransformation(mesh5, nvl::Translation3d(0, -5, 0));
    drawer3.update();
    drawer4.update();

    //Call of fit scene
    viewer.canvas()->fitScene();

    viewer.showMaximized(); //Show the viewer

    return app.exec();
}

