/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_CONVERT_H
#define NVL_MODELS_MESH_CONVERT_H

#include <nvl/nuvolib.h>

#ifdef NVL_VCGLIB_LOADED

#include <Eigen/Core>

#include <vector>

namespace nvl {

template<class VCGMesh, class Segment>
void convertSegmentsToVCGEdgeMesh(
        const std::vector<Segment>& segments,
        VCGMesh& vcgMesh);

#ifdef NVL_EIGEN_LOADED

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

#endif

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

#endif

#include "mesh_vcg_convert.cpp"

#endif // NVL_MODELS_MESH_CONVERT_H
