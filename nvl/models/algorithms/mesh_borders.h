/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_BORDERS_H
#define NVL_MODELS_MESH_BORDERS_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
bool meshIsBorderFaceEdge(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Index& edgePos);
template<class Mesh>
bool meshIsBorderFaceEdge(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Index& edgePos,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);

template<class Mesh>
bool meshIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId);
template<class Mesh>
bool meshIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);

template<class Mesh>
bool meshIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId);
template<class Mesh>
bool meshIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);


template<class Mesh>
std::vector<typename Mesh::FaceId> meshBorderFaces(
        const Mesh& mesh);
template<class Mesh>
std::vector<typename Mesh::FaceId> meshBorderFaces(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);

template<class Mesh>
std::vector<typename Mesh::VertexId> meshBorderVertices(
        const Mesh& mesh);
template<class Mesh>
std::vector<typename Mesh::VertexId> meshBorderVertices(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);

template<class Mesh>
std::vector<std::vector<typename Mesh::VertexId>> meshBorderVertexChains(
        const Mesh& mesh);
template<class Mesh>
std::vector<std::vector<typename Mesh::VertexId>> meshBorderVertexChains(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);



template<class Mesh, class Set>
bool meshSubsetIsBorderFaceEdge(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Index& edgePos,
        const Set& selectedFaces);
template<class Mesh, class Set>
bool meshSubsetIsBorderFaceEdge(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Index& edgePos,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        const Set& selectedFaces);

template<class Mesh, class Set>
bool meshSubsetIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Set& selectedFaces);
template<class Mesh, class Set>
bool meshSubsetIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        const Set& selectedFaces);

template<class Mesh, class Set>
bool meshSubsetIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId,
        const Set& selectedFaces);
template<class Mesh, class Set>
bool meshSubsetIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);

template<class Mesh, class Set>
std::vector<typename Mesh::FaceId> meshSubsetBorderFaces(
        const Mesh& mesh,
        const Set& selectedFaces);
template<class Mesh, class Set>
std::vector<typename Mesh::FaceId> meshSubsetBorderFaces(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);

template<class Mesh, class Set>
std::vector<typename Mesh::VertexId> meshSubsetBorderVertices(
        const Mesh& mesh,
        const Set& selectedFaces);
template<class Mesh, class Set>
std::vector<typename Mesh::VertexId> meshSubsetBorderVertices(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);

template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::VertexId>> meshSubsetBorderVertexChains(
        const Mesh& mesh,
        const Set& selectedFaces);
template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::VertexId>> meshSubsetBorderVertexChains(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);


}

#include "mesh_borders.cpp"

#endif // NVL_MODELS_MESH_BORDERS_H
