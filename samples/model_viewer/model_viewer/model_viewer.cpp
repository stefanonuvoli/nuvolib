#include <QApplication>

#include <nvl/viewer/viewerwindow.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/widgets/drawable_widget.h>
#include <nvl/viewer/widgets/model_drawer_widget.h>

#include <nvl/math/transformations.h>

#include <nvl/models/model_3d.h>
#include <nvl/models/algorithms/mesh_normals.h>
#include <nvl/models/algorithms/mesh_transformations.h>
#include <nvl/models/algorithms/mesh_geometric_information.h>
#include <nvl/models/io/model_io.h>

int main(int argc, char *argv[]) {
    typedef nvl::Model3d Model;
    typedef nvl::ModelDrawer<Model> ModelDrawer;

    QApplication app(argc, argv);

    nvl::ViewerWindow viewer;
    viewer.setWindowTitle("Model viewer sample");
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
    nvl::ModelDrawerWidget modelDrawerWidget(&canvas, &drawableListWidget);
    viewer.addToRightDock(&modelDrawerWidget);

    //Meshes
    Model model;

    //Load mesh
    nvl::modelLoadFromFile("../../data/cube.obj", model);

    //Translate mesh to the center
    nvl::AlignedBox3d bbox = nvl::meshBoundingBox(model.mesh);
    nvl::Vector3d center = -bbox.center();
    nvl::meshApplyTransformation(model.mesh, nvl::Translation3d(center));

    //Update the vertex and face normals
    model.mesh.computeFaceNormals();
    model.mesh.computeVertexNormals();

    //Initialize the drawers
    ModelDrawer drawer(&model);

    //Add to the canvas
    viewer.canvas()->addDrawable(&drawer);

    //Call of fit scene
    viewer.canvas()->fitScene();

    viewer.showMaximized(); //Show the viewer

    return app.exec();
}

