/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_split.h"

#include <nvl/models/algorithms/mesh_adjacencies.h>

namespace nvl {

/**
 * @brief Split an edge of a mesh
 * @param mesh Mesh
 * @param vertex1 First vertex id of the edge
 * @param vertex2 Second vertex id of the edge
 * @param newVertexPoint Point where to split
 * @return New vertex id
 */
template<class Mesh>
typename Mesh::VertexId meshSplitEdge(
        Mesh& mesh,
        const typename Mesh::VertexId& vertex1,
        const typename Mesh::VertexId& vertex2,
        const typename Mesh::Point& newVertexPoint)
{
    typedef typename Mesh::FaceId FaceId;

    std::vector<std::vector<FaceId>> vfAdj = meshVertexFaceAdjacencies(mesh);
    return meshSplitEdge(mesh, vertex1, vertex2, newVertexPoint, vfAdj);
}

/**
 * @brief Split an edge of a mesh
 * @param mesh Mesh
 * @param vertex1 First vertex id of the edge
 * @param vertex2 Second vertex id of the edge
 * @param newVertexPoint Point where to split
 * @param vfAdj Pre-computed vertex-face adjacencies (they will be updated)
 * @return New vertex id
 */
template<class Mesh>
typename Mesh::VertexId meshSplitEdge(
        Mesh& mesh,
        const typename Mesh::VertexId& vertex1,
        const typename Mesh::VertexId& vertex2,
        const typename Mesh::Point& newVertexPoint,
        std::vector<std::vector<typename Mesh::FaceId>>& vfAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;

    VertexId newVertexId = NULL_ID;

    std::vector<std::pair<FaceId, Index>> facesToSplit;
    for (const FaceId& fId : vfAdj[vertex1]) {
        const Face& face = mesh.face(fId);
        for (Index pos = 0; pos < face.vertexNumber(); ++pos) {
            const VertexId& v1 = face.vertexId(pos);
            const VertexId& v2 = face.nextVertexId(pos);
            if ((v1 == vertex1 && v2 == vertex2) || (v2 == vertex1 && v1 == vertex2)) {
                facesToSplit.push_back(std::make_pair(fId, pos));
            }
        }
    }

    if (!facesToSplit.empty()) {
        newVertexId = mesh.addVertex(newVertexPoint);
        vfAdj.push_back(std::vector<FaceId>());

        for (const std::pair<FaceId, Index>& entry : facesToSplit) {
            const FaceId& fId = entry.first;
            const Face& face = mesh.face(fId);

            const Index& pos = entry.second;

            std::vector<VertexId> faceVertices1(3);
            Index pos1 = (pos + 1) % face.vertexNumber();
            faceVertices1[0] = newVertexId;
            faceVertices1[1] = face.vertexId(pos1);
            faceVertices1[2] = face.nextVertexId(pos1);

            std::vector<VertexId> faceVertices2(3);
            Index pos2 = pos > 0 ? pos - 1 : face.vertexNumber() - 1;
            faceVertices2[0] = newVertexId;
            faceVertices2[1] = face.vertexId(pos2);
            faceVertices2[2] = face.nextVertexId(pos2);

            std::vector<VertexId> faceVertices3(face.vertexNumber() - 1);
            faceVertices3[0] = newVertexId;
            for (Index i = 1; i < face.vertexNumber() - 1; i++) {
                Index pos3 = (pos + 1 + i) % face.vertexNumber();
                faceVertices3[i] = face.vertexId(pos3);
            }

            FaceId fId1 = mesh.addFace(faceVertices1);

            FaceId fId2 = NULL_ID;
            if (faceVertices3.size() < 3) {
                mesh.setFaceVertexIds(fId, faceVertices2);
            }
            else {
                fId2 = mesh.addFace(faceVertices2);
                mesh.setFaceVertexIds(fId, faceVertices3);
            }

            vfAdj[newVertexId].push_back(fId);

            vfAdj[faceVertices1[1]].erase(std::remove(vfAdj[faceVertices1[1]].begin(), vfAdj[faceVertices1[1]].end(), fId), vfAdj[faceVertices1[1]].end());
            vfAdj[faceVertices1[0]].push_back(fId1);
            vfAdj[faceVertices1[1]].push_back(fId1);
            vfAdj[faceVertices1[2]].push_back(fId1);

            if (fId2 != NULL_ID) {
                vfAdj[faceVertices2[2]].erase(std::remove(vfAdj[faceVertices2[2]].begin(), vfAdj[faceVertices2[2]].end(), fId), vfAdj[faceVertices2[2]].end());
                vfAdj[faceVertices2[0]].push_back(fId2);
                vfAdj[faceVertices2[1]].push_back(fId2);
                vfAdj[faceVertices2[2]].push_back(fId2);
            }
        }
    }

    return newVertexId;
}

}
