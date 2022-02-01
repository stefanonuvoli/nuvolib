/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_IO_DATA_H
#define NVL_MODELS_MESH_IO_DATA_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>

#include <nvl/utilities/color.h>

#include <nvl/models/structures/mesh_material.h>

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
    bool vertexUVs;
    bool vertexColors;
    bool polylineColors;
    bool faceNormals;
    bool materials;
};

//Mesh data
template<class P = Point3d, class VN = Vector3d, class UV = Point2f, class VC = Color, class PC = Color, class FN = Vector3d, class M = MeshMaterial<Color>>
struct IOMeshData {
    IOMeshData();

    std::vector<P> vertices;
    std::vector<std::vector<Index>> faces;
    std::vector<std::vector<Index>> polylines;

    std::vector<VN> vertexNormals;
    std::vector<UV> vertexUVs;
    std::vector<VC> vertexColors;

    std::vector<PC> polylineColors;

    std::vector<FN> faceNormals;

    std::vector<M> materials;
    std::vector<Index> faceMaterials;

    std::vector<std::vector<Index>> faceVertexNormals;
    std::vector<std::vector<Index>> faceVertexUVs;

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
