#ifndef NVL_MODELS_MESH_IO_DATA_H
#define NVL_MODELS_MESH_IO_DATA_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>
#include <nvl/math/affine.h>
#include <nvl/utilities/color.h>
#include <nvl/models/mesh_material.h>

#include <vector>

namespace nvl {

//Error messages
enum IOMeshError { IO_MESH_SUCCESS, IO_MESH_FILE_ERROR, IO_MESH_FORMAT_NON_RECOGNISED, IO_MESH_EXTENSION_NON_SUPPORTED };

//IO mode
struct IOMeshMode {
    IOMeshMode();

    bool vertices;
    bool polylines;
    bool faces;
    bool vertexNormals;
    bool vertexUV;
    bool vertexColors;
    bool textures;
    bool faceNormals;
    bool materials;
};

//Mesh data
template<class P = Point3d, class VN = Vector3d, class VU = Point2f, class VC = Color, class FN = Vector3d, class M = MeshMaterial<Color>>
struct IOMeshData {
    IOMeshData();

    std::vector<P> vertices;
    std::vector<std::vector<Index>> faces;
    std::vector<std::vector<Index>> polylines;

    std::vector<VN> vertexNormals;
    std::vector<VU> vertexUV;
    std::vector<VC> vertexColors;

    std::vector<FN> faceNormals;

    std::vector<M> materials;
    std::vector<std::string> faceMaterials;

    std::vector<std::vector<Index>> faceVertexNormals;
    std::vector<std::vector<Index>> faceVertexUV;

    void clear();
};

namespace internal {

//Dummy variables for default arguments
static IOMeshError dummyIOMeshError;
static IOMeshMode dummyIOMeshMode;

}

}

#include "mesh_io_data.cpp"

#endif // NVL_MODELS_MESH_IO_DATA_H
