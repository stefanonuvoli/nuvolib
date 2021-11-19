#include <iostream>

#include <nvl/models/mesh_3d.h>
#include <nvl/models/algorithms/mesh_consistency.h>

/**
 * @brief Surface triangle mesh initialization and deletion samples.
 *
 * We create and initialize a triangle mesh, then we perform some delete operations.
 * We illustrate some operation for cleaning and guaranteeing the consistency of the mesh
 * after the delete operations.
 */
int main() {

    //Typedefs
    typedef nvl::TriangleMesh3d TriangleMesh3d;
    typedef TriangleMesh3d::VertexId VertexId;
    typedef TriangleMesh3d::FaceId FaceId;
    typedef TriangleMesh3d::PolylineId PolylineId;

    //Declare the mesh
    TriangleMesh3d mesh;

    //Creating a cube mesh
    std::cout << " >>> Creating a cube mesh" << std::endl;
    VertexId v0 = mesh.addVertex(-1.0, -1.0,  1.0);
    VertexId v1 = mesh.addVertex( 1.0, -1.0,  1.0);
    VertexId v2 = mesh.addVertex( 1.0,  1.0,  1.0);
    VertexId v3 = mesh.addVertex(-1.0,  1.0,  1.0);
    VertexId v4 = mesh.addVertex(-1.0, -1.0, -1.0);
    VertexId v5 = mesh.addVertex( 1.0, -1.0, -1.0);
    VertexId v6 = mesh.addVertex( 1.0,  1.0, -1.0);
    VertexId v7 = mesh.addVertex(-1.0,  1.0, -1.0);
    FaceId f0 =  mesh.addFace(v0, v1, v2);
    FaceId f1 =  mesh.addFace(v2, v3, v0);
    FaceId f2 =  mesh.addFace(v1, v5, v6);
    FaceId f3 =  mesh.addFace(v6, v2, v1);
    FaceId f4 =  mesh.addFace(v7, v6, v5);
    FaceId f5 =  mesh.addFace(v5, v4, v7);
    FaceId f6 =  mesh.addFace(v4, v0, v3);
    FaceId f7 =  mesh.addFace(v3, v7, v4);
    FaceId f8 =  mesh.addFace(v4, v5, v1);
    FaceId f9 =  mesh.addFace(v1, v0, v4);
    FaceId f10 = mesh.addFace(v3, v2, v6);
    FaceId f11 = mesh.addFace(v6, v7, v3);

    //Create some random polylines
    std::cout << " >>> Adding some random polylines" << std::endl;
    PolylineId p1 = mesh.addPolyline(v0, v1, v2, v3, v4);
    PolylineId p2 = mesh.addPolyline(v5, v6, v7);
    PolylineId p3 = mesh.addPolyline(v2, v6, v7);

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Delete some vertices
    std::cout << " >>> Deleting vertex 0..." << std::endl;
    mesh.deleteVertex(v0);
    std::cout << " >>> Deleting face 0..." << std::endl;
    mesh.deleteFace(f0);
    std::cout << " >>> Deleting vertex 1..." << std::endl;
    mesh.deleteVertex(v1);
    std::cout << " >>> Deleting face 1..." << std::endl;
    mesh.deleteFace(f1);
    std::cout << " >>> Deleting vertex 2..." << std::endl;
    mesh.deleteVertex(v2);
    std::cout << " >>> Deleting face 2..." << std::endl;
    mesh.deleteFace(v2);

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Compact vertex vector
    std::cout << " >>> Compacting vertices... " << std::endl;
    mesh.compactVertices();

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //We solve the consistency of the faces after vertex removal
    std::cout << " >>> Removing faces with deleted vertices... " << std::endl;
    nvl::meshRemoveFacesWithDeletedVertices(mesh); //This should be done after every vertex deletion!

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Compact face vector
    std::cout << " >>> Compacting faces..." << std::endl;
    mesh.compactFaces();

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //We solve the consistency of the polylines after vertex removal
    std::cout << " >>> Removing polylines with deleted vertices..." << std::endl;
    nvl::meshRemovePolylinesWithDeletedVertices(mesh); //This should be done after every vertex deletion!

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Compact polyline vector
    std::cout << " >>> Compacting polylines..." << std::endl;
    mesh.compactPolylines();

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Clear mesh vertices
    std::cout << " >>> Clear mesh vertices..." << std::endl;
    mesh.clearVertices();

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    std::cout << " >>> Removing faces with deleted vertices..." << std::endl;
    nvl::meshRemoveFacesWithDeletedVertices(mesh); //This should be done after every clear of vertices!
    std::cout << " >>> Removing polylines with deleted vertices..." << std::endl;
    nvl::meshRemovePolylinesWithDeletedVertices(mesh); //This should be done after every clear of vertices!

    //Print mesh
    std::cout << std::endl;
    std::cout << mesh << std::endl;



    //Suppressing warnings
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
