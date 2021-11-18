#include "mesh_morphological_operations.h"

#include <nvl/models/algorithms/mesh_adjacencies.h>
#include <nvl/models/algorithms/mesh_borders.h>

namespace nvl {

template<class Mesh, class Set>
void meshDilateFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces)
{
    typedef typename Mesh::FaceId FaceId;

    const std::vector<std::vector<FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshDilateFaceSelection(mesh, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
void meshDilateFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::VertexId VertexId;

    std::unordered_set<VertexId> verticesOnBorder;
    for (const FaceId& fId : selectedFaces) {
        assert(!mesh.isFaceDeleted(fId));

        const Face& face = mesh.face(fId);
        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            if (!meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj) && selectedFaces.find(ffAdj[fId][fePos]) == selectedFaces.end()) {
                verticesOnBorder.insert(face.vertexId(fePos));
                verticesOnBorder.insert(face.nextVertexId(fePos));
            }
        }
    }

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        bool inserted = false;
        for (Index fePos = 0; fePos < face.vertexNumber() && !inserted; ++fePos) {
            const VertexId& vId = face.vertexId(fePos);

            if (verticesOnBorder.find(vId) != verticesOnBorder.end()) {
                selectedFaces.insert(fId);
                inserted = true;
            }
        }
    }
}

template<class Mesh, class Set>
void meshErodeFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces)
{
    typedef typename Mesh::FaceId FaceId;

    const std::vector<std::vector<FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshErodeFaceSelection(mesh, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
void meshErodeFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::VertexId VertexId;

    std::unordered_set<VertexId> verticesNonSurrounded;
    for (const FaceId& fId : selectedFaces) {
        assert(!mesh.isFaceDeleted(fId));

        const Face& face = mesh.face(fId);
        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            if (meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj) || selectedFaces.find(ffAdj[fId][fePos]) == selectedFaces.end()) {
                verticesNonSurrounded.insert(face.vertexId(fePos));
                verticesNonSurrounded.insert(face.nextVertexId(fePos));
            }
        }
    }

    typename Set::iterator it = selectedFaces.begin();
    while (it != selectedFaces.end()) {
        const FaceId& fId = *it;
        assert(!mesh.isFaceDeleted(fId));

        const Face& face = mesh.face(fId);

        bool toDelete = false;
        for (Index fePos = 0; fePos < face.vertexNumber() && !toDelete; ++fePos) {
            if (verticesNonSurrounded.find(face.vertexId(fePos)) != verticesNonSurrounded.end()) {
                toDelete = true;
            }
        }

        if (toDelete)
            it = selectedFaces.erase(it);
        else
            it++;
    }
}

template<class Mesh, class Set>
void meshOpenFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces)
{
    typedef typename Mesh::FaceId FaceId;

    const std::vector<std::vector<FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshOpenFaceSelection(mesh, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
void meshOpenFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    meshErodeFaceSelection(mesh, selectedFaces, ffAdj);
    meshDilateFaceSelection(mesh, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
void meshCloseFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces)
{
    typedef typename Mesh::FaceId FaceId;

    const std::vector<std::vector<FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshCloseFaceSelection(mesh, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
void meshCloseFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    meshDilateFaceSelection(mesh, selectedFaces, ffAdj);
    meshErodeFaceSelection(mesh, selectedFaces, ffAdj);
}

}
