#ifndef NVL_MODELS_MESH_TRANSFER_H
#define NVL_MODELS_MESH_TRANSFER_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh>
void meshTransfer(
        const Mesh& mesh,
        Mesh& targetMesh);

template<class Mesh>
void meshTransfer(
        const Mesh& mesh,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline,
        std::vector<typename Mesh::FaceId>& birthFace);

template<class Mesh>
void meshTransferFaces(
        const Mesh& mesh,
        Mesh& targetMesh);

template<class Mesh>
void meshTransferFaces(
        const Mesh& mesh,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace);

template<class Mesh>
void meshTransferFaces(
        const Mesh& mesh,
        const std::vector<typename Mesh::FaceId>& faces,
        Mesh& targetMesh);

template<class Mesh>
void meshTransferFaces(
        const Mesh& mesh,
        const std::vector<typename Mesh::FaceId>& faces,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace);

template<class Mesh>
void meshTransferPolylines(
        const Mesh& mesh,
        Mesh& targetMesh);

template<class Mesh>
void meshTransferPolylines(
        const Mesh& mesh,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline);

template<class Mesh>
void meshTransferPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::PolylineId>& polylines,
        Mesh& targetMesh);

template<class Mesh>
void meshTransferPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::PolylineId>& polylines,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline);

template<class Mesh>
void meshTransferVertices(
        const Mesh& mesh,
        Mesh& targetMesh);

template<class Mesh>
void meshTransferVertices(
        const Mesh& mesh,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex);

template<class Mesh>
void meshTransferVertices(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh);

template<class Mesh>
void meshTransferVertices(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex);

template<class Mesh>
void meshTransferVerticesWithPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh);

template<class Mesh>
void meshTransferVerticesWithPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline);

template<class Mesh>
void meshTransferVerticesWithFaces(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh);

template<class Mesh>
void meshTransferVerticesWithFaces(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace);

template<class Mesh>
void meshTransferVerticesWithFacesAndPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh);

template<class Mesh>
void meshTransferVerticesWithFacesAndPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline,
        std::vector<typename Mesh::FaceId>& birthFace);

}

#include "mesh_transfer.cpp"

#endif // NVL_MODELS_MESH_TRANSFER_H
