/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_triangulation.h"

#include <nvl/models/algorithms/mesh_transfer.h>
#include <nvl/models/algorithms/mesh_subdivision.h>

#include <nvl/math/inverse_function.h>

#include <unordered_set>

namespace nvl {

/**
 * @brief Triangulate faces assuming they are convex
 * @param mesh Mesh
 */
template<class Mesh>
void meshTriangulateConvexFace(
        Mesh& mesh)
{
    typedef typename Mesh::FaceId FaceId;

    Size faceNumber = mesh.nextFaceId();

    for (FaceId fId = 0; fId < faceNumber; ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        std::vector<FaceId> resultingFaces = meshTriangulateConvexFace(mesh, fId);
    }
}

/**
 * @brief Triangulate faces assuming they are convex
 * @param mesh Mesh
 * @param birthFace Birth face of the resulting mesh
 */
template<class Mesh>
void meshTriangulateConvexFace(
        Mesh& mesh,
        std::vector<typename Mesh::FaceId>& birthFace)
{
    typedef typename Mesh::FaceId FaceId;

    Size faceNumber = mesh.nextFaceId();

    birthFace.resize(faceNumber);

    for (FaceId fId = 0; fId < faceNumber; ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        std::vector<FaceId> resultingFaces = meshTriangulateConvexFace(mesh, fId);

        for (const FaceId& resultingFaceId : resultingFaces) {
            if (resultingFaceId >= birthFace.size()) {
                birthFace.resize(resultingFaceId + 1);
            }
            birthFace[resultingFaceId] = fId;
        }
    }
}

/**
 * @brief Triangulate faces assuming they are convex
 * @param mesh Mesh
 * @param fId Face id
 */
template<class Mesh>
std::vector<typename Mesh::FaceId> meshTriangulateConvexFace(
        Mesh& mesh,
        const typename Mesh::FaceId& fId)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    const typename Mesh::FaceContainer faceVertices = mesh.faceVertexIds(fId);
    const Face face = mesh.face(fId);

    Size vertexNumber = faceVertices.size();

    std::vector<FaceId> resultingFaces;

    if (vertexNumber > 3) {
        resultingFaces.resize(vertexNumber - 2);
        resultingFaces[0] = fId;

        for (VertexId j = 0; j < vertexNumber - 2; ++j) {
            FaceId currentFaceId = fId;

            if (j > 0) {
                currentFaceId = mesh.addFace(face);
                resultingFaces[j] = currentFaceId;
            }

            std::vector<VertexId> newVertices(3);
            newVertices[0] = faceVertices[0];
            newVertices[1] = faceVertices[j + 1];
            newVertices[2] = faceVertices[(j + 2) % vertexNumber];

            mesh.setFaceVertexIds(currentFaceId, newVertices);
        }
    }
    else {
        resultingFaces.resize(1);
        resultingFaces[0] = fId;
    }

    return resultingFaces;
}

/**
 * @brief Triangulate faces splitting on barycenter
 * @param mesh Mesh
 */
template<class Mesh>
void meshTriangulateBarycenter(
        Mesh& mesh)
{
    typedef typename Mesh::FaceId FaceId;

    Size faceNumber = mesh.nextFaceId();

    for (FaceId fId = 0; fId < faceNumber; ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        std::vector<FaceId> resultingFaces = meshTriangulateBarycenter(mesh, fId);
    }
}

/**
 * @brief Triangulate faces splitting on barycenter
 * @param mesh Mesh
 * @param birthFace Birth face of the resulting mesh
 */
template<class Mesh>
void meshTriangulateBarycenter(
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

/**
 * @brief Triangulate faces splitting in the barycenter
 * @param mesh Mesh
 * @param fId Face id
 */
template<class Mesh>
std::vector<typename Mesh::FaceId> meshTriangulateBarycenter(
        Mesh& mesh,
        const typename Mesh::FaceId& fId)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    Face& face = mesh.face(fId);
    std::vector<VertexId> faceVertices(face.vertexIds().begin(), face.vertexIds().end());

    Index vertexNumber = faceVertices.size();

    std::vector<FaceId> resultingFaces;

    if (vertexNumber > 3) {
        resultingFaces = meshSubdivideInBarycenterWithTriangles(mesh, fId);
    }
    else {
        resultingFaces.resize(1);
        resultingFaces[0] = fId;
    }

    return resultingFaces;
}

/**
 * @brief Reconstruct polygons from a triangulated mesh, where they have not changed
 * @param originalMesh Original mesh
 * @param derivedMesh Derived mesh
 * @param derivedBirthVertex Birth vertex of the derived mesh
 * @param derivedBirthFace Birth face of the derived mesh
 * @param resultBirthFace Birth vertex of the resulting mesh
 * @return Reconstructed mesh
 */
template<class Mesh>
Mesh meshReconstructTriangulatedPolygons(
        const Mesh& originalMesh,
        const Mesh& derivedMesh,
        const std::vector<typename Mesh::VertexId>& derivedBirthVertex,
        const std::vector<typename Mesh::FaceId>& derivedBirthFace,
        std::vector<typename Mesh::FaceId>& resultBirthFace)
{
    //TODO general reconstruction in proper file
    //TODO to test

    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    Mesh resultMesh;

    //Copy vertices
    std::vector<VertexId> transferBirthVertices;
    meshTransferVertices(derivedMesh, resultMesh, transferBirthVertices);

    //Get the sets of vertices in faces of the original mesh
    std::vector<std::unordered_set<VertexId>> originalFaceSet(originalMesh.nextFaceId());
    for (FaceId fId = 0; fId < originalMesh.nextFaceId(); ++fId) {
        if (originalMesh.isFaceDeleted(fId))
            continue;

        const Face& face = originalMesh.face(fId);

        for (VertexId j = 0; j < face.vertexNumber(); ++j) {
            originalFaceSet[fId].insert(face.vertexId(j));
        }
    }

    //Get the sets of vertices in faces of the derived mesh
    std::vector<std::unordered_set<VertexId>> derivedFaceSet(originalMesh.nextFaceId());
    for (FaceId fId = 0; fId < derivedMesh.nextFaceId(); ++fId) {
        if (derivedMesh.isFaceDeleted(fId))
            continue;

        const Face& face = derivedMesh.face(fId);
        const FaceId originalFaceId = derivedBirthFace[fId];

        if (originalFaceId != NULL_ID) {
            for (VertexId j = 0; j < face.vertexNumber(); ++j) {
                const VertexId originalVertexId = derivedBirthVertex[face.vertexId(j)];
                derivedFaceSet[originalFaceId].insert(originalVertexId);
            }
        }
    }

    //Get the faces to copy
    std::unordered_set<FaceId> derivedFacesToCopy;
    std::unordered_set<FaceId> originalFacesToCopy;
    for (FaceId fId = 0; fId < derivedMesh.nextFaceId(); ++fId) {
        if (derivedMesh.isFaceDeleted(fId))
            continue;

        const FaceId originalFaceId = derivedBirthFace[fId];

        if (originalFaceId != NULL_ID && originalFaceSet[originalFaceId] == derivedFaceSet[originalFaceId]) {
            originalFacesToCopy.insert(originalFaceId);
        }
        else {
            derivedFacesToCopy.insert(fId);
        }
    }

    //Get inverse of the derived birth vertex
    std::vector<VertexId> derivedVertexMap = inverseFunction(derivedBirthVertex, originalMesh.nextVertexId());
    std::vector<VertexId> transferVertexMap = inverseFunction(transferBirthVertices, derivedMesh.nextVertexId());

    //Add derived faces
    for (const FaceId& fId : derivedFacesToCopy) {
        const Face& face = derivedMesh.face(fId);

        std::vector<VertexId> newVertices(face.vertexNumber());

        for (VertexId j = 0; j < face.vertexNumber(); ++j) {
            const VertexId vId = transferVertexMap[face.vertexId(j)];
            assert(vId != NULL_ID);
            newVertices[j] = vId;
        }

        FaceId newFId = resultMesh.addFace(face);
        resultMesh.setFaceVertexIds(newFId, newVertices);

        assert(resultBirthFace.size() == newFId);
        resultBirthFace.push_back(NULL_ID);
    }

    //Add original faces
    for (const FaceId& fId : originalFacesToCopy) {
        const Face& face = originalMesh.face(fId);

        std::vector<VertexId> newVertices(face.vertexNumber());

        for (VertexId j = 0; j < face.vertexNumber(); ++j) {
            if (derivedVertexMap[face.vertexId(j)] != NULL_ID) {
                const VertexId vId = transferVertexMap[derivedVertexMap[face.vertexId(j)]];
                assert(vId != NULL_ID);
                newVertices[j] = vId;
            }
        }

        FaceId newFId = resultMesh.addFace(face);
        resultMesh.setFaceVertexIds(newFId, newVertices);

        assert(resultBirthFace.size() == newFId);
        resultBirthFace.push_back(fId);
    }

    return resultMesh;
}


}
