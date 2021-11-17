#include <QApplication>

#include <nvl/viewer/viewerwindow.h>
#include <nvl/viewer/widgets/qglviewer_canvas.h>
#include <nvl/viewer/drawables/face_mesh_drawer.h>

#include <nvl/models/meshes.h>
#include <nvl/models/mesh_normals.h>
#include <nvl/models/mesh_subdivision.h>

#include <nvl/math/barycentric_coordinates.h>
#include <nvl/math/barycentric_interpolation.h>
#include <nvl/math/translation.h>

#define SUBDIVISION_ITERATIONS 3

template<class Mesh>
void createMesh(Mesh& mesh);

template<class MeshDrawer>
void configureMeshDrawer(MeshDrawer& meshDrawer);

int main(int argc, char *argv[])
{
    typedef nvl::TriangleMesh3d Mesh;
    typedef nvl::FaceMeshDrawer<Mesh> MeshDrawer;

    QApplication app(argc, argv);

    //Viewer
    nvl::ViewerWindow viewer;
    viewer.setWindowTitle("SkinMixer");

    //Add canvas
    nvl::QGLViewerCanvas canvas(&viewer);
    viewer.addCanvas(&canvas);

    //Create mesh
    Mesh mesh;
    createMesh(mesh);



    /* ------------------ SUBDIVISION AND INTERPOLATION ------------------ */

    //Subdivision
    Mesh subdividedMesh = mesh;
    for (int i = 0; i < SUBDIVISION_ITERATIONS; i++) {
        nvl::meshSubdivideInTrianglesBarycenter(subdividedMesh);
    }

    //Interpolate colors
    for (Mesh::Vertex& v : subdividedMesh.vertices()) {
        //Get barycentric coordinates with respect to the first mesh face (0, 1, 2)
        std::vector<Mesh::Scalar> bc = nvl::barycentricCoordinates(
            mesh.vertex(0).point(),
            mesh.vertex(1).point(),
            mesh.vertex(2).point(),
            v.point());

        //Interpolate colors with barycentric coordinates
        float red = nvl::barycentricInterpolation(
            mesh.vertexColor(0).redF(),
            mesh.vertexColor(1).redF(),
            mesh.vertexColor(2).redF(),
            bc);
        float green = nvl::barycentricInterpolation(
            mesh.vertexColor(0).greenF(),
            mesh.vertexColor(1).greenF(),
            mesh.vertexColor(2).greenF(),
            bc);
        float blue = nvl::barycentricInterpolation(
            mesh.vertexColor(0).blueF(),
            mesh.vertexColor(1).blueF(),
            mesh.vertexColor(2).blueF(),
            bc);

        //Set the color
        subdividedMesh.setVertexColor(v, nvl::Color(red, green, blue));
    }

    /* ------------------------------------------------------------------- */



    //Compute normals
    mesh.computeNormals();
    subdividedMesh.computeNormals();

    //Add to canvas the mesh drawers
    MeshDrawer meshDrawer(&mesh);
    MeshDrawer subdividedMeshDrawer(&subdividedMesh);
    configureMeshDrawer(meshDrawer);
    configureMeshDrawer(subdividedMeshDrawer);

    //We translate the frame in order to view the meshes side by side
    meshDrawer.setFrame(nvl::Translation3d(-0.7, 0.0, 0.0) * meshDrawer.frame());
    subdividedMeshDrawer.setFrame(nvl::Translation3d(0.7, 0.0, 0.0) * subdividedMeshDrawer.frame());

    //Add to canvas and fit the scene
    canvas.addDrawable(&meshDrawer);
    canvas.addDrawable(&subdividedMeshDrawer);
    canvas.fitScene();

    //Show viewer
    viewer.showMaximized();

    return app.exec();
}

template<class Mesh>
void createMesh(Mesh& mesh)
{
   typedef typename Mesh::VertexId VertexId;

   //Create vertices
   VertexId v1 = mesh.addVertex(0.0, 1.0, 0.0);
   VertexId v2 = mesh.addVertex(-0.5, 0.0, 0.0);
   VertexId v3 = mesh.addVertex(0.5, 0.0, 0.0);

   //Create face
   mesh.addFace(v1, v2, v3);

   //Set vertex colors
   mesh.enableVertexColors();
   mesh.setVertexColor(v1, nvl::Color(1.0, 0.0, 0.0));
   mesh.setVertexColor(v2, nvl::Color(0.0, 1.0, 0.0));
   mesh.setVertexColor(v3, nvl::Color(0.0, 0.0, 1.0));
}

template<class MeshDrawer>
void configureMeshDrawer(MeshDrawer& meshDrawer)
{
    meshDrawer.setVertexVisible(true);
    meshDrawer.setFaceShadingMode(nvl::FaceMeshDrawer<nvl::TriangleMesh3d>::FACE_SHADING_SMOOTH);
    meshDrawer.setFaceColorMode(nvl::FaceMeshDrawer<nvl::TriangleMesh3d>::FACE_COLOR_PER_VERTEX);
}

