/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_NORMALS_H
#define NVL_MODELS_MESH_NORMALS_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
void meshComputeFaceNormalsPlanar(Mesh& mesh, const bool normalize = true);
template<class Mesh>
void meshComputeFaceNormalPlanar(Mesh& mesh, typename Mesh::FaceId& fId, const bool normalize = true);

template<class Mesh>
void meshComputeFaceNormalsSVDFitting(Mesh& mesh, const bool normalize = true);
template<class Mesh>
void meshComputeFaceNormalSVDFitting(Mesh& mesh, typename Mesh::FaceId& fId, const bool normalize = true);

template<class Mesh>
void meshComputeFaceNormalsCovarianceFitting(Mesh& mesh, const bool normalize = true);
template<class Mesh>
void meshComputeFaceNormalCovarianceFitting(Mesh& mesh, typename Mesh::FaceId& fId, const bool normalize = true);

template<class Mesh>
void meshComputeFaceNormalsFromVertexNormals(Mesh& mesh, const bool normalize = true);
template<class Mesh>
void meshComputeFaceNormalFromVertexNormals(Mesh& mesh, typename Mesh::FaceId& fId, const bool normalize = true);

template<class Mesh>
void meshComputeVertexNormalsFromFaceNormals(Mesh& mesh, const bool normalize = true);
template<class Mesh>
void meshComputeVertexNormalFromFaceNormals(Mesh& mesh, typename Mesh::VertexId& vId, const bool normalize = true);
template<class Mesh>
void meshComputeVertexNormalsFromFaceNormals(
        Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj,
        bool normalize = true);
template<class Mesh>
void meshComputeVertexNormalFromFaceNormals(
        Mesh& mesh,
        typename Mesh::VertexId& vId,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj,
        bool normalize = true);

}

#include "mesh_normals.cpp"

#endif // NVL_MODELS_MESH_NORMALS_H
