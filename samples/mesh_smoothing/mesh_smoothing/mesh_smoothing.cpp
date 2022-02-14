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
#include <nvl/models/algorithms/mesh_normals.h>
#include <nvl/models/algorithms/mesh_transformations.h>
#include <nvl/models/algorithms/mesh_geometric_information.h>
#include <nvl/models/algorithms/mesh_smoothing.h>
#include <nvl/models/algorithms/mesh_smoothing_reprojection.h>
#include <nvl/models/io/mesh_io.h>

//DELETE
#include <chrono>

int main(int argc, char *argv[]) {
    typedef nvl::PolygonMesh3d Mesh;
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
    nvl::VertexMeshDrawerWidget vertexMeshDrawerWidget(&canvas, &drawableListWidget);
    viewer.addToRightDock(&vertexMeshDrawerWidget);

    //Meshes
    Mesh mesh1, mesh2, mesh3, mesh4, mesh5;

    //Load mesh
    bool success = nvl::meshLoadFromFile("../../data/bunny_5000.obj", mesh1);
    if (!success) {
        std::cout << "Impossible to load mesh." << std::endl;
        exit(1);
    }

    //Translate mesh to the center
    nvl::AlignedBox3d bbox = nvl::meshBoundingBox(mesh1);
    nvl::Vector3d center = -bbox.center();
    double scaleFactor = 1.0 / bbox.diagonal().norm();

    nvl::meshApplyTransformation(mesh1, nvl::Translation3d(center));
    nvl::meshApplyTransformation(mesh1, nvl::Scaling3d(scaleFactor, scaleFactor, scaleFactor));

    //Update the vertex and face normals
    mesh1.computeFaceNormals();
    mesh1.computeVertexNormals();

    //Copy the same mesh
    mesh5 = mesh4 = mesh3 = mesh2 = mesh1;

    //We apply a transformation to the first three meshes
    double offsetX = (bbox.max()(0) - bbox.min()(0)) * 1.1;
    double offsetY = (bbox.max()(1) - bbox.min()(1)) * 1.1;
    nvl::meshApplyTransformation(mesh1, nvl::Translation3d(-offsetX, -offsetY/2, 0));
    nvl::meshApplyTransformation(mesh3, nvl::Translation3d(+offsetX, 0, 0));
    nvl::meshApplyTransformation(mesh4, nvl::Translation3d(0, -offsetY, 0));
    nvl::meshApplyTransformation(mesh5, nvl::Translation3d(+offsetX, -offsetY, 0));

    //Smoothing
    nvl::meshLaplacianSmoothing(mesh2, 20, 0.7);
    nvl::meshCotangentSmoothing(mesh3, 20, 0.7);

    //DELETE
    using namespace std;
    using namespace std::chrono;
        auto start = high_resolution_clock::now();

    nvl::meshLaplacianSmoothingReprojection(mesh4, 20, 0.4);
    nvl::meshCotangentSmoothingReprojection(mesh5, 20, 0.4);

    // Get ending timepoint
    //DELETE
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    //DELETE
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

    //Initialize the drawers
    FaceMeshDrawer drawer1(&mesh1);
    FaceMeshDrawer drawer2(&mesh2);
    FaceMeshDrawer drawer3(&mesh3);
    FaceMeshDrawer drawer4(&mesh4);
    FaceMeshDrawer drawer5(&mesh5);
    drawer1.setWireframeVisible(true);
    drawer1.setWireframeSize(2);
    drawer2.setWireframeVisible(true);
    drawer2.setWireframeSize(2);
    drawer3.setWireframeVisible(true);
    drawer3.setWireframeSize(2);
    drawer4.setWireframeVisible(true);
    drawer4.setWireframeSize(2);
    drawer5.setWireframeVisible(true);
    drawer5.setWireframeSize(2);

    //Add to the canvas
    viewer.canvas()->addDrawable(&drawer1, "Original");
    viewer.canvas()->addDrawable(&drawer2, "Laplacian");
    viewer.canvas()->addDrawable(&drawer3, "Cotangent laplacian");
    viewer.canvas()->addDrawable(&drawer4, "Laplacian reprojection");
    viewer.canvas()->addDrawable(&drawer5, "Cotangent laplacian reprojection");

    //Fit the scene
    viewer.canvas()->fitScene();

    //Show the viewer
    viewer.showMaximized();

    return app.exec();
}

