#include <iostream>

#include <nvl/models/meshes.h>
#include <nvl/models/mesh_normals.h>

/**
 * @brief We set the geometric face and vertex normals of a mesh
 */
int main() {

    //Typedefs
    typedef nvl::PolygonMesh3d PolygonMesh3d;
    typedef PolygonMesh3d::Vertex Vertex;
    typedef PolygonMesh3d::VertexId VertexId;
    typedef PolygonMesh3d::Face Face;
    typedef PolygonMesh3d::VertexNormal VertexNormal;
    typedef PolygonMesh3d::FaceNormal FaceNormal;

    //Declare the mesh
    PolygonMesh3d mesh;

    //Create vertices
    VertexId v0 = mesh.addVertex(-1.0, -1.0,  1.0);
    VertexId v1 = mesh.addVertex( 1.0, -1.0,  1.0);
    VertexId v2 = mesh.addVertex( 1.0,  1.0,  1.0);
    VertexId v3 = mesh.addVertex(-1.0,  1.0,  1.0);
    VertexId v4 = mesh.addVertex(-1.0, -1.0, -1.0);
    VertexId v5 = mesh.addVertex( 1.0, -1.0, -1.0);
    VertexId v6 = mesh.addVertex( 1.0,  1.0, -1.0);
    VertexId v7 = mesh.addVertex(-1.0,  1.0, -1.0);

    //Create some polygon faces
    mesh.addFace(v0, v1, v2, v3);
    mesh.addFace(v1, v5, v6, v2);
    mesh.addFace(v7, v6, v5, v4);

    //Some triangle faces
    mesh.addFace(v4, v0, v3);
    mesh.addFace(v3, v7, v4);
    mesh.addFace(v4, v5, v1);
    mesh.addFace(v1, v0, v4);
    mesh.addFace(v3, v2, v6);
    mesh.addFace(v6, v7, v3);

    //Print the polygon mesh
    std::cout << " >>> Mesh:" << std::endl;
    std::cout << std::endl << mesh << std::endl;

    //Compute face normals (using SVD fitting for polygon faces)
    //Also the default "meshUpdateFaceNormals(mesh);" uses this method
    nvl::meshComputeFaceNormalsSVDFitting(mesh);

    //Print the face normals
    std::cout << "Face normals:" << std::endl;
    for (const Face& face : mesh.faces()) {
        FaceNormal& normal = mesh.faceNormal(face);
        std::cout << face.id() << " -> " << normal.transpose() << std::endl;
    }
    std::cout << std::endl;

    //Compute vertex normals (using face normal averages)
    //Also the default "meshUpdateVertexNormals(mesh);" uses this method
    nvl::meshComputeVertexNormalsFromFaceNormals(mesh);

    //Print the vertex normals
    std::cout << "Vertex normals:" << std::endl;
    for (const Vertex& vertex : mesh.vertices()) {
        VertexNormal& normal = mesh.vertexNormal(vertex);
        std::cout << vertex.id() << " -> " << normal.transpose() << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
