#include "mesh_subdivision.h"

#include <nvl/math/barycenter.h>
#include <nvl/models/mesh_adjacencies.h>
#include <nvl/models/mesh_split.h>

namespace nvl {


template<class Mesh>
void meshSubdivideInTrianglesBarycenter(
        Mesh& mesh)
{
    typedef typename Mesh::FaceId FaceId;

    Size faceNumber = mesh.nextFaceId();

    for (FaceId fId = 0; fId < faceNumber; ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        std::vector<FaceId> resultingFaces = meshSubdivideInTrianglesBarycenter(mesh, fId);
    }
}

template<class Mesh>
void meshSubdivideInTrianglesBarycenter(
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
std::vector<typename Mesh::FaceId> meshSubdivideInTrianglesBarycenter(
        Mesh& mesh,
        const typename Mesh::FaceId& fId)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    const Face& face = mesh.face(fId);

    std::vector<VertexId> faceVertices(face.vertexIds().begin(), face.vertexIds().end());

    Index vertexNumber = faceVertices.size();

    std::vector<FaceId> resultingFaces(vertexNumber);
    resultingFaces[0] = fId;

    Point b = mesh.vertexPoint(face.vertexId(0));
    for (Index j = 1; j < vertexNumber; ++j) {
        b += mesh.vertexPoint(face.vertexId(j));
    }
    b /= vertexNumber;

    VertexId newVertexId = mesh.addVertex(b);

    for (VertexId j = 0; j < vertexNumber; ++j) {
        FaceId currentFaceId = fId;
        if (j > 0) {
            const Face& currentFace = mesh.face(currentFaceId);
            FaceId newFaceId = mesh.addFace(currentFace);
            currentFaceId = newFaceId;
            resultingFaces[j] = newFaceId;
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
