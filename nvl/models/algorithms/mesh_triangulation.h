/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_TRIANGULATION_H
#define NVL_MODELS_MESH_TRIANGULATION_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
void meshTriangulateConvexFace(
        Mesh& mesh);
template<class Mesh>
void meshTriangulateConvexFace(
        Mesh& mesh,
        std::vector<typename Mesh::FaceId>& birthFace);
template<class Mesh>
std::vector<typename Mesh::FaceId> meshTriangulateConvexFace(
        Mesh& mesh,
        const typename Mesh::FaceId& fId);

template<class Mesh>
void meshTriangulateBarycenter(
        Mesh& mesh);
template<class Mesh>
void meshTriangulateBarycenter(
        Mesh& mesh,
        std::vector<typename Mesh::FaceId>& birthFace);
template<class Mesh>
std::vector<typename Mesh::FaceId> meshTriangulateBarycenter(
        Mesh& mesh,
        const typename Mesh::FaceId& fId);


//TODO general reconstruction in proper file
template<class Mesh>
Mesh meshReconstructTriangulatedPolygons(
        const Mesh& originalMesh,
        const Mesh& derivedMesh,
        const std::vector<typename Mesh::VertexId>& derivedBirthVertex,
        const std::vector<typename Mesh::FaceId>& derivedBirthFace,
        std::vector<typename Mesh::FaceId>& resultBirthFace);

}

#include "mesh_triangulation.cpp"

#endif // NVL_MODELS_MESH_TRIANGULATION_H
