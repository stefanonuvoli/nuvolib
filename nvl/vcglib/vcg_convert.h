#ifndef NVL_VCGLIB_VCG_CONVERT_H
#define NVL_VCGLIB_VCG_CONVERT_H

#include <nvl/nuvolib.h>

#include <Eigen/Core>

#include <vector>

namespace nvl {

template<class VCGMesh, class Segment>
void convertSegmentsToVCGEdgeMesh(
        const std::vector<Segment>& segments,
        VCGMesh& vcgMesh);

template<class VCGMesh>
void convertVCGMeshToEigen(
        VCGMesh& vcgMesh,
        Eigen::MatrixXd& V,
        Eigen::MatrixXi& F,
        std::vector<Index>& vMap,
        std::vector<Index>& fMap,
        bool selectedOnly = false,
        Index numVerticesPerFace = 3,
        Index dim = 3);

template<class VCGMesh>
void convertEigenToVCGMesh(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& F,
        VCGMesh& vcgMesh);

template<class Mesh, class VCGMesh>
void convertMeshToVCGMesh(
        const Mesh& mesh,
        VCGMesh& vcgMesh,
        Index dim = 3);

template<class Mesh, class VCGMesh>
void convertMeshToVCGMesh(
        const Mesh& mesh,
        VCGMesh& vcgMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace,
        Index dim = 3);

template<class Mesh, class VCGMesh>
void convertVCGMeshToMesh(
        VCGMesh& vcgMesh,
        Mesh& mesh,
        bool selectedOnly = false,
        Index dim = 3);

template<class Mesh, class VCGMesh>
void convertVCGMeshToMesh(
        VCGMesh& vcgMesh,
        Mesh& mesh,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace,
        bool selectedOnly = false,
        Index dim = 3);

}

#include "vcg_convert.cpp"

#endif // NVL_VCGLIB_VCG_CONVERT_H
