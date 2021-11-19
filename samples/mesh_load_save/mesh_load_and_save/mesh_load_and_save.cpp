#include <iostream>

#include <nvl/models/mesh_3d.h>
#include <nvl/models/algorithms/mesh_normals.h>
#include <nvl/models/io/mesh_io.h>

nvl::TriangleMesh3d createTriangleCube();
nvl::PolygonMesh3d createPolygonCube();

/**
 * @brief Load and save of a polygon mesh
 *
 * We load and save a mesh given a file.
 *
 */
int main() {


    //Typedefs

    typedef nvl::TriangleMesh3d TriangleMesh3d;
    typedef nvl::PolygonMesh3d PolygonMesh3d;
    typedef TriangleMesh3d::VertexId VertexId;
    typedef TriangleMesh3d::FaceId FaceId;
    typedef TriangleMesh3d::PolylineId PolylineId;

    //Declare the meshes
    TriangleMesh3d triangleMesh;
    PolygonMesh3d polygonMesh;

    //Load-save info and success info (they are optional)
    bool success;
    nvl::IOMeshError loadError;
    nvl::IOMeshError saveError;

    std::cout << " >>> Loading triangle mesh..." << std::endl;
    success = meshLoadFromFile("../../data/cube.obj", triangleMesh, loadError);

    if (success) { //Mesh correctly loaded
        std::cout << " >>> Mesh loaded!" << std::endl;
    }
    else {
        assert(loadError != nvl::IO_MESH_SUCCESS);

        switch (loadError) {
        case nvl::IO_MESH_FILE_ERROR:
            std::cout << " >>> Could not load mesh for a file opening error!" << std::endl;
            break;
        case nvl::IO_MESH_FORMAT_NON_RECOGNISED:
            std::cout << " >>> Could not load mesh for a file format error!" << std::endl;
            break;
        default:
            std::cout << " >>> Could not load mesh for an unknown error!" << std::endl;
            break;
        }

        //Create the triangle mesh of a cube
        std::cout << " >>> Creating triangle mesh..." << std::endl;
        triangleMesh = createTriangleCube();

        //Save the triangle mesh
        std::cout << " >>> Saving triangle mesh..." << std::endl;
        success = nvl::meshSaveToFile("../../data/cube.obj", triangleMesh);

        //Reload the triangle mesh
        std::cout << " >>> Reloading triangle mesh..." << std::endl;
        meshLoadFromFile("../../data/cube.obj", triangleMesh);
    }

    std::cout << " >>> Loaded triangle mesh:" << std::endl;
    std::cout << std::endl << triangleMesh << std::endl;

    //Create the polygon mesh
    std::cout << " >>> Creating polygon mesh..." << std::endl;
    polygonMesh = createPolygonCube();

    //Save the polygon mesh (only geometry!)
    nvl::IOMeshMode saveMode;
    saveMode.materials = false;
    saveMode.vertexColors = false;
    saveMode.vertexNormals = false;
    std::cout << " >>> Saving polygon mesh..." << std::endl;
    meshSaveToFile("../../data/polygoncube.obj", polygonMesh, saveError, saveMode);

    //Reload the polygon mesh (only geometry!)
    nvl::IOMeshMode loadMode;
    loadMode.materials = false;
    loadMode.vertexColors = false;
    loadMode.vertexNormals = false;
    std::cout << " >>> Reloading polygon mesh..." << std::endl;
    meshLoadFromFile("../../data/polygoncube.obj", polygonMesh, loadError, loadMode);

    //Print the polygon mesh
    std::cout << " >>> Loaded polygon mesh:" << std::endl;
    std::cout << std::endl << polygonMesh << std::endl;

    return 0;
}


/**
 * @brief Function to create a simple mesh triangle mesh of a cube.
 * We add some parameters
 */
nvl::TriangleMesh3d createTriangleCube() {
    typedef nvl::TriangleMesh3d Mesh;
    typedef Mesh::VertexId VertexId;
    typedef Mesh::FaceId FaceId;
    typedef Mesh::Material Material;
    typedef Mesh::MaterialId MaterialId;
    typedef Mesh::WedgeNormal WedgeNormal;
    typedef Mesh::WedgeNormalId WedgeNormalId;

    Mesh mesh;

    //Enable components
    mesh.enableFaceMaterials();
    mesh.enableVertexNormals();
    mesh.enableVertexColors();
    mesh.enableFaceNormals();
    mesh.enableWedgeNormals();

    //Create vertices
    VertexId v0 =  mesh.addVertex(-1.0, -1.0,  1.0);
    VertexId v1 =  mesh.addVertex( 1.0, -1.0,  1.0);
    VertexId v2 =  mesh.addVertex( 1.0,  1.0,  1.0);
    VertexId v3 =  mesh.addVertex(-1.0,  1.0,  1.0);
    VertexId v4 =  mesh.addVertex(-1.0, -1.0, -1.0);
    VertexId v5 =  mesh.addVertex( 1.0, -1.0, -1.0);
    VertexId v6 =  mesh.addVertex( 1.0,  1.0, -1.0);
    VertexId v7 =  mesh.addVertex(-1.0,  1.0, -1.0);
    VertexId v8 =  mesh.addVertex(-1.0,  2.0, -1.0);
    VertexId v9 =  mesh.addVertex( 1.0,  2.0, -1.0);
    VertexId v10 = mesh.addVertex( 1.0,  2.0,  1.0);
    VertexId v11 = mesh.addVertex(-1.0,  2.0,  1.0);

    //Add vertex colors
    mesh.setVertexColor(v0, nvl::RED);
    mesh.setVertexColor(v1, nvl::YELLOW);
    mesh.setVertexColor(v2, nvl::BLACK);
    mesh.setVertexColor(v3, nvl::BLUE);
    mesh.setVertexColor(v4, nvl::Color(0.8, 0.8, 0.8));

    //Create a polyline
    mesh.addPolyline(v8, v9, v10, v11, v8);

    //Create faces
    FaceId f0 = mesh.addFace(v0, v1, v2);
    FaceId f1 = mesh.addFace(v2, v3, v0);
    FaceId f2 = mesh.addFace(v1, v5, v6);
    FaceId f3 = mesh.addFace(v6, v2, v1);
    FaceId f4 = mesh.addFace(v7, v6, v5);
    FaceId f5 = mesh.addFace(v5, v4, v7);
    FaceId f6 = mesh.addFace(v4, v0, v3);
    FaceId f7 = mesh.addFace(v3, v7, v4);
    FaceId f8 = mesh.addFace(v4, v5, v1);
    FaceId f9 = mesh.addFace(v1, v0, v4);
    FaceId f10 = mesh.addFace(v3, v2, v6);
    FaceId f11 = mesh.addFace(v6, v7, v3);

    //Create new materials
    Material material1;
    material1.setName("redmaterial");
    material1.setDiffuseColor(nvl::RED);
    MaterialId m1 = mesh.addMaterial(material1);

    Material material2;
    material2.setName("greenmaterial");
    material2.setDiffuseColor(nvl::GREEN);
    MaterialId m2 = mesh.addMaterial(material2);

    Material material3;
    material3.setName("bluematerial");
    material3.setDiffuseColor(nvl::BLUE);
    MaterialId m3 = mesh.addMaterial(material3);

    Material material4;
    material4.setName("yellowmaterial");
    material4.setDiffuseColor(nvl::YELLOW);
    MaterialId m4 = mesh.addMaterial(material4);

    Material material5;
    material5.setName("blackmaterial");
    material5.setDiffuseColor(nvl::BLACK);
    MaterialId m5 = mesh.addMaterial(material5);

    Material material6;
    material6.setName("greymaterial");
    material6.setDiffuseColor(nvl::Color(0.8, 0.8, 0.8));
    MaterialId m6 = mesh.addMaterial(material6);

    //Set a material to a face
    mesh.setFaceMaterialId(f0, m1);
    mesh.setFaceMaterialId(f1, m1);
    mesh.setFaceMaterialId(f2, m2);
    mesh.setFaceMaterialId(f3, m2);
    mesh.setFaceMaterialId(f4, m3);
    mesh.setFaceMaterialId(f5, m3);
    mesh.setFaceMaterialId(f6, m4);
    mesh.setFaceMaterialId(f7, m4);
    mesh.setFaceMaterialId(f8, m5);
    mesh.setFaceMaterialId(f9, m5);
    mesh.setFaceMaterialId(f10, m6);
    mesh.setFaceMaterialId(f11, m6);

    //Delete a material
    mesh.deleteMaterial(m6);

    //Delete face material for faces who had the 6th material
    mesh.unsetFaceMaterialId(f10);
    mesh.setFaceMaterialId(f11, nvl::NULL_ID); //Equivalent to unset

    //We add custom wedge vertex normal for the two first vertices in a face
    WedgeNormalId wn1 = mesh.addWedgeNormal(WedgeNormal(1,0,0));
    WedgeNormalId wn2 = mesh.addWedgeNormal(WedgeNormal(1,1,0));
    std::vector<WedgeNormalId> wedgeNormals = {wn1, wn2, nvl::NULL_ID}; //The third will use vertex normals (if enabled)
    mesh.setFaceWedgeNormals(f11, wedgeNormals);

    //Compute vertex and face normals
    mesh.computeFaceNormals();
    mesh.computeVertexNormals();

    return mesh;
}


/**
 * @brief Function to create a simple mesh polygon mesh of a cube
 */
nvl::PolygonMesh3d createPolygonCube() {
    typedef nvl::PolygonMesh3d Mesh;
    typedef Mesh::VertexId VertexId;

    Mesh mesh;

    //Create vertices
    VertexId v0 = mesh.addVertex(-1.0, -1.0,  1.0);
    VertexId v1 = mesh.addVertex( 1.0, -1.0,  1.0);
    VertexId v2 = mesh.addVertex( 1.0,  1.0,  1.0);
    VertexId v3 = mesh.addVertex(-1.0,  1.0,  1.0);
    VertexId v4 = mesh.addVertex(-1.0, -1.0, -1.0);
    VertexId v5 = mesh.addVertex( 1.0, -1.0, -1.0);
    VertexId v6 = mesh.addVertex( 1.0,  1.0, -1.0);
    VertexId v7 = mesh.addVertex(-1.0,  1.0, -1.0);

    //Create faces
    mesh.addFace(v0, v1, v2, v3);
    mesh.addFace(v1, v5, v6, v2);
    mesh.addFace(v7, v6, v5, v4);
    mesh.addFace(v4, v0, v3, v7);
    mesh.addFace(v4, v5, v1, v0);
    mesh.addFace(v3, v2, v6, v7);

    return mesh;
}
