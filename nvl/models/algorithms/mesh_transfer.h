/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_TRANSFER_H
#define NVL_MODELS_MESH_TRANSFER_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {


/* Transfer entire mesh */

template<class M1, class M2>
void meshTransfer(
        const M1& mesh,
        M2& targetMesh);

template<class M1, class M2>
void meshTransfer(
        const M1& mesh,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline,
        std::vector<typename M1::FaceId>& birthFace);



/* Transfer vertices */

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        M2& targetMesh);

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex);

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh);

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex);

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::VertexId>& vertexMap);



/* Transfer polylines */

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        M2& targetMesh);

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline);

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        const std::vector<typename M1::PolylineId>& polylines,
        M2& targetMesh);

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        const std::vector<typename M1::PolylineId>& polylines,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline);

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        const std::vector<typename M1::PolylineId>& polylines,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline,
        std::vector<typename M1::VertexId>& vertexMap);

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertexMap,
        M2& targetMesh,
        std::vector<typename M1::PolylineId>& birthPolyline);



/* Transfer faces */

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        M2& targetMesh);

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::FaceId>& birthFace);

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        const std::vector<typename M1::FaceId>& faces,
        M2& targetMesh);

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        const std::vector<typename M1::FaceId>& faces,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::FaceId>& birthFace);

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        const std::vector<typename M1::FaceId>& faces,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::FaceId>& birthFace,
        std::vector<typename M1::VertexId>& vertexMap);

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertexMap,
        M2& targetMesh,
        std::vector<typename M1::FaceId>& birthFace);



/* Transfer vertices with related components */

template<class M1, class M2>
void meshTransferVerticesWithPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh);

template<class M1, class M2>
void meshTransferVerticesWithPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline);

template<class M1, class M2>
void meshTransferVerticesWithFaces(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh);

template<class M1, class M2>
void meshTransferVerticesWithFaces(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::FaceId>& birthFace);

template<class M1, class M2>
void meshTransferVerticesWithFacesAndPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh);

template<class M1, class M2>
void meshTransferVerticesWithFacesAndPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline,
        std::vector<typename M1::FaceId>& birthFace);

}

#include "mesh_transfer.cpp"

#endif // NVL_MODELS_MESH_TRANSFER_H
