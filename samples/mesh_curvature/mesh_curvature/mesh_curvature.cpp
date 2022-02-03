#include <QApplication>

#include <nvl/viewer/viewerwindow.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/widgets/drawable_widget.h>
#include <nvl/viewer/widgets/face_mesh_drawer_widget.h>
#include <nvl/viewer/widgets/vertex_mesh_drawer_widget.h>
#include <nvl/viewer/drawables/face_mesh_drawer.h>
#include <nvl/viewer/drawables/principal_curvature_drawer.h>
#include <nvl/viewer/interfaces/drawable.h>

#include <nvl/math/transformations.h>
#include <nvl/math/common_functions.h>

#include <nvl/models/mesh_3d.h>
#include <nvl/models/algorithms/mesh_normals.h>
#include <nvl/models/algorithms/mesh_transformations.h>
#include <nvl/models/algorithms/mesh_geometric_information.h>
#include <nvl/models/algorithms/mesh_curvature.h>
#include <nvl/models/io/mesh_io.h>

#include <nvl/utilities/color_utils.h>

int main(int argc, char *argv[]) {
    typedef nvl::PolygonMesh3d Mesh;
    typedef nvl::FaceMeshDrawer<Mesh> FaceMeshDrawer;
    typedef nvl::PrincipalCurvatureDrawer<Mesh> PrincipalCurvatureDrawer;

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
    nvl::meshApplyTransformation(mesh1, nvl::Translation3d(-offsetX, 0, 0));
    nvl::meshApplyTransformation(mesh3, nvl::Translation3d(+offsetX, 0, 0));
    nvl::meshApplyTransformation(mesh4, nvl::Translation3d(-offsetX, -offsetY, 0));
    nvl::meshApplyTransformation(mesh5, nvl::Translation3d(0, -offsetY, 0));

    //Mean curvature (mesh1)
    std::vector<Mesh::Scalar> mcVCG = nvl::meshMeanCurvatureVCG(mesh1);
    mesh1.enableVertexColors();
    Mesh::Scalar minValue1 = nvl::min(mcVCG);
    Mesh::Scalar maxValue1 = nvl::max(mcVCG);
    for (const Mesh::Vertex& v : mesh1.vertices()) {
        nvl::Color color;
        if (mcVCG[v.id()] < 0) {
            color = nvl::colorRampRedGreen(-nvl::pow(-mcVCG[v.id()], 1.5), minValue1, 0.0);
        }
        else {
            color = nvl::colorRampGreenBlue(nvl::pow(mcVCG[v.id()], 1.5), 0.0, maxValue1);
        }
        mesh1.setVertexColor(v, color);
    }

    //Mean curvature (mesh2)
    std::vector<Mesh::Scalar> gcVCG = nvl::meshGaussianCurvatureVCG(mesh2);
    mesh2.enableVertexColors();
    Mesh::Scalar minValue2 = nvl::min(gcVCG);
    Mesh::Scalar maxValue2 = nvl::max(gcVCG);
    for (const Mesh::Vertex& v : mesh2.vertices()) {
        nvl::Color color;
        if (gcVCG[v.id()] < 0) {
            color = nvl::colorRampRedGreen(-nvl::pow(-gcVCG[v.id()], 1.5), minValue2, 0.0);
        }
        else {
            color = nvl::colorRampGreenBlue(nvl::pow(gcVCG[v.id()], 1.5), 0.0, maxValue2);
        }
        mesh2.setVertexColor(v, color);
    }

    //Principal curvature directions (mesh3)
    std::vector<nvl::PrincipalCurvatures<typename Mesh::Scalar>> pcVCG = nvl::meshPrincipalCurvatureVCG(mesh3);



    //Mean curvature (mesh4)
    std::vector<Mesh::Scalar> mc = nvl::meshMeanCurvatureLB(mesh4);
    mesh4.enableVertexColors();
    Mesh::Scalar minValue4 = nvl::min(mc);
    Mesh::Scalar maxValue4 = nvl::max(mc);
    for (const Mesh::Vertex& v : mesh4.vertices()) {
        nvl::Color color;
        if (mc[v.id()] < 0) {
            color = nvl::colorRampRedGreen(-nvl::pow(-mc[v.id()], 1.5), minValue4, 0.0);
        }
        else {
            color = nvl::colorRampGreenBlue(nvl::pow(mc[v.id()], 1.5), 0.0, maxValue4);
        }
        mesh4.setVertexColor(v, color);
    }

    //Mean curvature (mesh5)
    std::vector<Mesh::Scalar> gc = nvl::meshGaussianCurvatureLB(mesh5);
    mesh5.enableVertexColors();
    Mesh::Scalar minValue5 = nvl::min(gc);
    Mesh::Scalar maxValue5 = nvl::max(gc);
    for (const Mesh::Vertex& v : mesh5.vertices()) {
        nvl::Color color;
        if (gc[v.id()] < 0) {
            color = nvl::colorRampRedGreen(-nvl::pow(-gc[v.id()], 1.5), minValue5, 0.0);
        }
        else {
            color = nvl::colorRampGreenBlue(nvl::pow(gc[v.id()], 1.5), 0.0, maxValue5);
        }
        mesh5.setVertexColor(v, color);
    }


    //Initialize the drawers
    FaceMeshDrawer drawer1(&mesh1);
    FaceMeshDrawer drawer2(&mesh2);
    FaceMeshDrawer drawer3(&mesh3);
    PrincipalCurvatureDrawer vcgPCDrawer(&mesh3, &pcVCG);
    FaceMeshDrawer drawer4(&mesh4);
    FaceMeshDrawer drawer5(&mesh5);

    drawer1.setWireframeVisible(true);
    drawer1.setWireframeSize(1);
    drawer1.setFaceColorMode(FaceMeshDrawer::FACE_COLOR_PER_VERTEX);

    drawer2.setWireframeVisible(true);
    drawer2.setWireframeSize(1);
    drawer2.setFaceColorMode(FaceMeshDrawer::FACE_COLOR_PER_VERTEX);

    drawer3.setWireframeVisible(true);
    drawer3.setWireframeSize(1);
    drawer3.setFaceColorMode(FaceMeshDrawer::FACE_COLOR_UNIFORM);

    drawer4.setWireframeVisible(true);
    drawer4.setWireframeSize(1);
    drawer4.setFaceColorMode(FaceMeshDrawer::FACE_COLOR_PER_VERTEX);

    drawer5.setWireframeVisible(true);
    drawer5.setWireframeSize(1);
    drawer5.setFaceColorMode(FaceMeshDrawer::FACE_COLOR_PER_VERTEX);


    //Add to the canvas
    viewer.canvas()->addDrawable(&drawer1, "Mean curvature VCG");
    viewer.canvas()->addDrawable(&drawer2, "Gaussian curvature VCG");
    viewer.canvas()->addDrawable(&drawer3, "Principal direction curvature VCG");
    viewer.canvas()->addDrawable(&vcgPCDrawer, "Principal direction curvature", false);
    viewer.canvas()->addDrawable(&drawer4, "Mean curvature nuvolib");
    viewer.canvas()->addDrawable(&drawer5, "Gaussian curvature nuvolib");

    //Fit the scene
    viewer.canvas()->fitScene();

    //Show the viewer
    viewer.showMaximized();

    return app.exec();
}

