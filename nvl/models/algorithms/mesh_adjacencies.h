#ifndef NVL_MODELS_MESH_ADJACENCIES_H
#define NVL_MODELS_MESH_ADJACENCIES_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
std::vector<std::vector<typename Mesh::VertexId>> meshVertexVertexAdjacencies(
        const Mesh& mesh,
        const bool& faces = true,
        const bool& polylines = true);

template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshVertexFaceAdjacencies(
        const Mesh& mesh);
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshVertexFaceAdjacencies(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& vfPos);


template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshFaceFaceAdjacencies(
        const Mesh& mesh);
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshFaceFaceAdjacencies(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vfAdj);

template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh);
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh,
        std::vector<Index>& faceComponentMap);
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        std::vector<Index>& faceComponentMap);

template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces);
template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);
template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces,
        std::vector<Index>& faceComponentMap);
template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        std::vector<Index>& faceComponentMap);

}

#include "mesh_adjacencies.cpp"

#endif // NVL_MODELS_MESH_ADJACENCIES_H
