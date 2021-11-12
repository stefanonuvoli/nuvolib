#include <QApplication>

#include <nvl/viewer/viewerwindow.h>
#include <nvl/viewer/widgets/qglviewer_canvas.h>
#include <nvl/viewer/drawables/face_mesh_drawer.h>

#include <nvl/models/meshes.h>
#include <nvl/models/mesh_normals.h>
#include <nvl/models/mesh_subdivision.h>

#include <nvl/math/barycentric_interpolation.h>

#include <nvl/math/translation.h>

#define SUBDIVISION_ITERATIONS 3

template<class Mesh>
void createMesh(Mesh& mesh);

template<class MeshDrawer>
void configureMeshDrawer(MeshDrawer& meshDrawer);

int main(int argc, char *argv[])
{
    typedef nvl::PolygonMesh3d Mesh;
    typedef nvl::FaceMeshDrawer<Mesh> MeshDrawer;

    QApplication app(argc, argv);

    //Viewer and canvas initialization
    nvl::ViewerWindow viewer;
    viewer.setWindowTitle("SkinMixer");
    nvl::QGLViewerCanvas canvas(&viewer);
    viewer.addCanvas(&canvas);

    //Create mesh
    Mesh mesh;
    createMesh(mesh);



    /* ------------------ SUBDIVISION AND INTERPOLATION ------------------ */

    //Subdivision
    Mesh subdividedMesh1 = mesh;
    Mesh subdividedMesh2 = mesh;
    for (int i = 0; i < SUBDIVISION_ITERATIONS; i++) {
        nvl::meshSubdivideInTrianglesBarycenter(subdividedMesh1);
        nvl::meshSubdivideInTrianglesBarycenter(subdividedMesh2);
    }


    //Get polygon and color values of the first (and only) face of the mesh
    const Mesh::Face& face = mesh.face(0);

    std::vector<Mesh::Point> polygon(face.vertexNumber());
    std::vector<float> redValues(face.vertexNumber());
    std::vector<float> greenValues(face.vertexNumber());
    std::vector<float> blueValues(face.vertexNumber());

    for (Mesh::VertexId j = 0; j < face.vertexNumber(); ++j) {
        const Mesh::Vertex& v = mesh.vertex(face.vertexId(j));

        polygon[j] = v.point();
        redValues[j] = v.color().redF();
        greenValues[j] = v.color().greenF();
        blueValues[j] = v.color().blueF();
    }

    for (Mesh::Vertex& v : subdividedMesh1.vertices()) {
        //Interpolation on polygon using diagonal subdivision
        float red = nvl::barycentricInterpolationDiagonalSubdivision(
                    polygon,
                    v.point(),
                    redValues);
        float green = nvl::barycentricInterpolationDiagonalSubdivision(
                    polygon,
                    v.point(),
                    greenValues);
        float blue = nvl::barycentricInterpolationDiagonalSubdivision(
                    polygon,
                    v.point(),
                    blueValues);

        //Set the color
        v.setColor(nvl::Color(red, green, blue));
    }

    for (Mesh::Vertex& v : subdividedMesh2.vertices()) {
        //Interpolation on polygon using barycenter subdivision
        float red = nvl::barycentricInterpolationBarycenterSubdivision(
                    polygon,
                    v.point(),
                    redValues);
        float green = nvl::barycentricInterpolationBarycenterSubdivision(
                    polygon,
                    v.point(),
                    greenValues);
        float blue = nvl::barycentricInterpolationBarycenterSubdivision(
                    polygon,
                    v.point(),
                    blueValues);

        //Set the color
        v.setColor(nvl::Color(red, green, blue));
    }

    /* ------------------------------------------------------------------- */



    //Compute normals
    mesh.computeNormals();
    subdividedMesh1.computeNormals();
    subdividedMesh2.computeNormals();

    //Add the mesh drawers to the canvas
    MeshDrawer meshDrawer(&mesh);
    MeshDrawer subdividedMeshDrawer1(&subdividedMesh1);
    MeshDrawer subdividedMeshDrawer2(&subdividedMesh2);
    configureMeshDrawer(meshDrawer);
    configureMeshDrawer(subdividedMeshDrawer1);
    configureMeshDrawer(subdividedMeshDrawer2);

    //We translate the frame in order to view the meshes side by side
    meshDrawer.setFrame(nvl::Translation3d(-4, 0.0, 0.0) * meshDrawer.frame());
    subdividedMeshDrawer2.setFrame(nvl::Translation3d(4, 0.0, 0.0) * subdividedMeshDrawer2.frame());

    //Add to canvas and fit the scene
    canvas.addDrawable(&meshDrawer);
    canvas.addDrawable(&subdividedMeshDrawer1);
    canvas.addDrawable(&subdividedMeshDrawer2);
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
   VertexId v1 = mesh.addVertex(1.0, 1.0, 0.0);
   VertexId v2 = mesh.addVertex(-1.0, 1.5, 0.0);
   VertexId v3 = mesh.addVertex(-1.5, -1.2, 0.0);
   VertexId v4 = mesh.addVertex(2.0, -0.5, 0.0);

   //Create face
   mesh.addFace(v1, v2, v3, v4);

   //Set vertex colors
   mesh.setVertexColor(v1, nvl::Color(1.0, 0.0, 0.0));
   mesh.setVertexColor(v2, nvl::Color(0.0, 1.0, 0.0));
   mesh.setVertexColor(v3, nvl::Color(0.0, 0.0, 1.0));
   mesh.setVertexColor(v4, nvl::Color(0.0, 0.0, 0.0));
}

template<class MeshDrawer>
void configureMeshDrawer(MeshDrawer& meshDrawer)
{
    meshDrawer.setVertexVisible(true);
    meshDrawer.setFaceShadingMode(nvl::FaceMeshDrawer<nvl::TriangleMesh3d>::FACE_SHADING_SMOOTH);
    meshDrawer.setFaceColorMode(nvl::FaceMeshDrawer<nvl::TriangleMesh3d>::FACE_COLOR_PER_VERTEX);
}
