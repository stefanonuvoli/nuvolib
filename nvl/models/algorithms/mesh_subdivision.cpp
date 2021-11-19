#include "mesh_subdivision.h"

#include <nvl/math/barycenter.h>
#include <nvl/models/algorithms/mesh_adjacencies.h>
#include <nvl/models/algorithms/mesh_split.h>

#include <nvl/models/algorithms/mesh_geometric_information.h>

namespace nvl {


template<class Mesh>
void meshSubdivideInBarycenterWithTriangles(
        Mesh& mesh)
{
    typedef typename Mesh::FaceId FaceId;

    Size faceNumber = mesh.nextFaceId();

    for (FaceId fId = 0; fId < faceNumber; ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        std::vector<FaceId> resultingFaces = meshSubdivideInBarycenterWithTriangles(mesh, fId);
    }
}

template<class Mesh>
void meshSubdivideInBarycenterWithTriangles(
        Mesh& mesh,
        std::vector<typename Mesh::FaceId>& birthFace)
{
    typedef typename Mesh::FaceId FaceId;

    Size faceNumber = mesh.nextFaceId();

    birthFace.resize(faceNumber);

    for (FaceId fId = 0; fId < faceNumber; ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        std::vector<FaceId> resultingFaces = meshTriangulateBarycenter(mesh, fId);

        for (const FaceId& resultingFaceId : resultingFaces) {
            if (resultingFaceId >= birthFace.size()) {
                birthFace.resize(resultingFaceId + 1);
            }
            birthFace[resultingFaceId] = fId;
        }
    }
}

template<class Mesh>
std::vector<typename Mesh::FaceId> meshSubdivideInBarycenterWithTriangles(
        Mesh& mesh,
        const typename Mesh::FaceId& fId)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    const typename Mesh::FaceContainer faceVertices = mesh.faceVertexIds(fId);
    const Face face = mesh.face(fId);

    Index vertexNumber = faceVertices.size();

    Point b = meshFaceBarycenter(mesh, fId);

    VertexId newVertexId = mesh.addVertex(b);

    std::vector<FaceId> resultingFaces(vertexNumber);
    resultingFaces[0] = fId;
    for (VertexId j = 0; j < vertexNumber; ++j) {
        FaceId currentFaceId = fId;

        if (j > 0) {
            currentFaceId = mesh.addFace(face);
            resultingFaces[j] = currentFaceId;
        }

        std::vector<VertexId> newVertices(3);
        newVertices[0] = faceVertices[j];
        newVertices[1] = faceVertices[(j + 1) % vertexNumber];
        newVertices[2] = newVertexId;

        mesh.setFaceVertexIds(currentFaceId, newVertices);
    }

    return resultingFaces;
}

}
