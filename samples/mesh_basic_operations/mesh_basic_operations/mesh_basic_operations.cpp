#include <iostream>

#include <nvl/models/mesh_3d.h>
#include <nvl/models/algorithms/mesh_consistency.h>

/**
 * @brief Surface triangle mesh initialization and deletion samples.
 *
 * We create and initialize a triangle mesh, then we perform some delete operations.
 * We illustrate some operation for cleaning and guarantee the consistency of the mesh
 * after the delete operations.
 */
int main() {

    //Typedefs
    typedef nvl::PolygonMesh3d PolygonMesh3d;
    typedef PolygonMesh3d::VertexId VertexId;
    typedef PolygonMesh3d::FaceId FaceId;
    typedef PolygonMesh3d::PolylineId PolylineId;
    typedef PolygonMesh3d::Vertex Vertex;
    typedef PolygonMesh3d::Face Face;
    typedef PolygonMesh3d::Polyline Polyline;
    typedef PolygonMesh3d::Point Point;

    //Declare the mesh
    PolygonMesh3d mesh;

    //Create vertices inserting points
    VertexId v0 = mesh.addVertex(Point(-1.0, -1.0,  1.0));
    VertexId v1 = mesh.addVertex(Point( 1.0, -1.0,  1.0));
    VertexId v2 = mesh.addVertex(Point( 1.0,  1.0,  1.0));
    VertexId v3 = mesh.addVertex(Point(-1.0,  1.0,  1.0));
    //Create vertices directly inserting vertices
    VertexId v4 = mesh.addVertex(Vertex(-1.0, -1.0, -1.0));
    VertexId v5 = mesh.addVertex(Vertex(1.0, -1.0, -1.0));
    //Create vertices directly inserting coordinates
    VertexId v6 = mesh.addVertex( 1.0,  1.0, -1.0);
    VertexId v7 = mesh.addVertex( 0.0,  0,    0);



    //Referencing vertices
    const Vertex& vertex4 = mesh.vertex(v4);
    Vertex& vertex7 = mesh.vertex(v7);

    //Edit vertex coordinates (equal operations)
    mesh.vertex(v7).setPoint(Point(-2.0,  2.0, -2.0));
    vertex7.setPoint(-1.0, 1.0, -1.0);



    //Create faces using vertices ids
    FaceId f0 =  mesh.addFace(v0, v1, v2);
    FaceId f1 =  mesh.addFace(v2, v3, v0);
    FaceId f2 =  mesh.addFace(v1, v5, v6);
    FaceId f3 =  mesh.addFace(v6, v2, v1);
    //Create faces using explicit faces
    FaceId f4 =  mesh.addFace(Face(7, 6, 5));
    FaceId f5 =  mesh.addFace(Face(5, 4, 7));
    //Create faces using explicit ids
    FaceId f6 =  mesh.addFace(4, 0, 3);
    FaceId f7 =  mesh.addFace(0.5, 0, 0.0f); //This should be avoided! Implicit cast
    //Create faces using different kind of entries
    FaceId f8 =  mesh.addFace(vertex4, 5, v1);
    FaceId f9 =  mesh.addFace(v1, 0, vertex4.id());
    FaceId f10 = mesh.addFace(v3, 2, v6);
    FaceId f11 = mesh.addFace(v6, 7, v3);


    //Referencing a face
    Face& face7 = mesh.face(f7);

    //Edit face (equal operations)
    mesh.face(f7).setVertexIds(v3, v7, v4);
    face7.setVertexIds(3, vertex7.id(), vertex4);



    //We add some polylines
    PolylineId p1 = mesh.addPolyline(v0, v1, v2, v3, v4);
    PolylineId p2 = mesh.addPolyline(v5, v6, v7);
    PolylineId p3 = mesh.addPolyline(2, v6, vertex7.id());

    //Referencing a polyline
    Polyline& polyline3 = mesh.polyline(p3);

    //Edit polyline (equal operations)
    mesh.polyline(p3).setVertexIds(v3, v7, v4, v5);
    polyline3.setVertexIds(3, vertex7.id(), vertex4, v5);

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Delete using ids
    std::cout << " >>> Deleting vertex 0..." << std::endl;
    mesh.deleteVertex(v0);
    std::cout << " >>> Deleting face 0..." << std::endl;
    mesh.deleteFace(0);

    //Delete using referencies
    std::cout << " >>> Deleting vertex 1..." << std::endl;
    const Vertex& vertex1 = mesh.vertex(1);
    mesh.deleteVertex(vertex1);
    std::cout << " >>> Deleting face 1..." << std::endl;
    const Face& face1 = mesh.face(1);
    mesh.deleteFace(face1);

    //Delete using id attributes
    std::cout << " >>> Deleting vertex 4..." << std::endl;
    mesh.deleteVertex(mesh.vertex(4).id());
    std::cout << " >>> Deleting face 4..." << std::endl;
    mesh.deleteFace(mesh.face(4).id());

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Add 2 random polygonal faces
    std::cout << " >>> Adding 2 random polygonal faces..." << std::endl;
    mesh.addFace(v0, v1, v2, v3); //We are inserting a polyline with a deleted vertex!
    mesh.addFace(v2, v3, v5, v6, v7);

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Solving all the unconsistent components
    std::cout << " >>> Solving all the unconsistent components..." << std::endl;
    meshRemoveFacesWithDeletedVertices(mesh);
    meshRemovePolylinesWithDeletedVertices(mesh);

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Compacting everything
    std::cout << " >>> Compacting everything..." << std::endl;
    mesh.compactAll();

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Clear mesh
    std::cout << " >>> Clear mesh..." << std::endl;
    mesh.clear();

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;


    NVL_SUPPRESS_UNUSEDVARIABLE(f0);
    NVL_SUPPRESS_UNUSEDVARIABLE(f1);
    NVL_SUPPRESS_UNUSEDVARIABLE(f2);
    NVL_SUPPRESS_UNUSEDVARIABLE(f3);
    NVL_SUPPRESS_UNUSEDVARIABLE(f4);
    NVL_SUPPRESS_UNUSEDVARIABLE(f5);
    NVL_SUPPRESS_UNUSEDVARIABLE(f6);
    NVL_SUPPRESS_UNUSEDVARIABLE(f7);
    NVL_SUPPRESS_UNUSEDVARIABLE(f8);
    NVL_SUPPRESS_UNUSEDVARIABLE(f9);
    NVL_SUPPRESS_UNUSEDVARIABLE(f10);
    NVL_SUPPRESS_UNUSEDVARIABLE(f11);
    NVL_SUPPRESS_UNUSEDVARIABLE(p1);
    NVL_SUPPRESS_UNUSEDVARIABLE(p2);
    NVL_SUPPRESS_UNUSEDVARIABLE(p3);

    return 0;
}
