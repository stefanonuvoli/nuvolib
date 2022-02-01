/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MESH_EIGEN_CONVERT_H
#define NVL_MESH_EIGEN_CONVERT_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Core>

#include <vector>

namespace nvl {

template<class Mesh>
void convertMeshToEigenMesh(
        const Mesh& mesh,
        Eigen::MatrixXd& V,
        Eigen::MatrixXi& F,
        Size dim = 3,
        Size numVerticesPerFace = 3);
template<class Mesh>
void convertMeshToEigenMesh(
        const Mesh& mesh,
        Eigen::MatrixXd& V,
        Eigen::MatrixXi& F,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace,
        Size dim = 3,
        Size numVerticesPerFace = 3);

template<class Mesh>
void convertEigenMeshToMesh(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& F,
        Mesh& mesh);
template<class Mesh>
void convertEigenMeshToMesh(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& F,
        Mesh& mesh,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace);

}

#endif

#include "mesh_eigen_convert.cpp"

#endif // NVL_MESH_EIGEN_CONVERT_H
