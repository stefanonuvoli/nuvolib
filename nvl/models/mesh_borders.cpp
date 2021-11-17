#include "mesh_borders.h"

#include <unordered_set>

namespace nvl {

template<class Mesh>
bool meshIsBorderFaceEdge(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Index& fePos)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj);
}

template<class Mesh>
bool meshIsBorderFaceEdge(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Index& fePos,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);

    return ffAdj[fId][fePos] == NULL_ID;
}

template<class Mesh>
bool meshIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshIsBorderFace(mesh, fId, ffAdj);
}

template<class Mesh>
bool meshIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::Face Face;

    const Face& face = mesh.face(fId);

    for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
        if (meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj)) {
            return true;
        }
    }

    return false;
}

template<class Mesh>
bool meshIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshIsBorderVertex(mesh, vId, ffAdj);
}

template<class Mesh>
bool meshIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;


    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);
        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            if (face.vertexId(fePos) == vId || face.nextVertexId(fePos) == vId) {
                if (meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj)) {
                    return true;
                }
            }
        }
    }

    return false;
}

template<class Mesh>
std::vector<typename Mesh::FaceId> meshBorderFaces(const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshBorderFaces(mesh, ffAdj);
}

template<class Mesh>
std::vector<typename Mesh::FaceId> meshBorderFaces(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::FaceId FaceId;

    std::vector<FaceId> borderFaces;

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId)) {
            continue;
        }

        if (meshIsBorderFace(mesh, fId, ffAdj)) {
            borderFaces.push_back(fId);
        }
    }

    return borderFaces;
}

template<class Mesh>
std::vector<typename Mesh::VertexId> meshBorderVertices(const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshBorderVertices(mesh, ffAdj);
}

template<class Mesh>
std::vector<typename Mesh::VertexId> meshBorderVertices(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;

    std::unordered_set<VertexId> borderVertices;

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId)) {
            continue;
        }

        const Face& face = mesh.face(fId);

        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            if (meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj)) {
                borderVertices.insert(face.vertexId(fePos));
                borderVertices.insert(face.nextVertexId(fePos));
            }
        }
    }

    return std::vector<VertexId>(borderVertices.begin(), borderVertices.end());
}

template<class Mesh>
std::vector<std::vector<typename Mesh::VertexId>> meshBorderVertexChains(
        const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshBorderVertexChains(mesh, ffAdj);
}

template<class Mesh>
std::vector<std::vector<typename Mesh::VertexId>> meshBorderVertexChains(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;

    std::vector<std::vector<VertexId>> chains;

    std::vector<std::vector<VertexId>> nextMap(mesh.nextVertexId());

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId)) {
            continue;
        }

        const Face& face = mesh.face(fId);

        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            if (meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj)) {
                VertexId vId = face.vertexId(fePos);
                VertexId nextVId = face.nextVertexId(fePos);
                nextMap[vId].push_back(nextVId);
            }
        }
    }

    std::vector<std::vector<bool>> visited(mesh.nextVertexId());
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId)) {
            continue;
        }

        visited[vId].resize(nextMap[vId].size(), false);
    }

    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId)) {
            continue;
        }

        //If it is a border vertex
        if (!nextMap[vId].empty()) {
            std::vector<VertexId> chain = internal::findBorderChainLoop(vId, vId, nextMap, visited);

            if (!chain.empty()) {
                chains.push_back(chain);
            }
        }
    }

    return chains;
}

template<class Mesh, class Set>
bool meshSubsetIsBorderFaceEdge(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Index& fePos,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetIsBorderFaceEdge(mesh, fId, fePos, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
bool meshSubsetIsBorderFaceEdge(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Index& fePos,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);

    return ffAdj[fId][fePos] == NULL_ID || selectedFaces.find(ffAdj[fId][fePos]) == selectedFaces.end();
}

template<class Mesh, class Set>
bool meshSubsetIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetIsBorderFace(mesh, fId, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
bool meshSubsetIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::Face Face;

    const Face& face = mesh.face(fId);

    for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
        if (meshIsBorderFaceEdge(mesh, fId, fePos, selectedFaces, ffAdj)) {
            return true;
        }
    }

    return false;
}

template<class Mesh, class Set>
bool meshSubsetIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetIsBorderVertex(mesh, vId, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
bool meshSubsetIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;


    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);
        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            if (face.vertexId(fePos) == vId || face.nextVertexId(fePos) == vId) {
                if (meshIsBorderFaceEdge(mesh, fId, fePos, selectedFaces, ffAdj)) {
                    return true;
                }
            }
        }
    }

    return false;
}

template<class Mesh, class Set>
std::vector<typename Mesh::VertexId> meshSubsetBorderFaces(
        const Mesh& mesh,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetBorderFaces(mesh, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
std::vector<typename Mesh::VertexId> meshSubsetBorderFaces(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::FaceId FaceId;

    std::vector<FaceId> borderFaces;

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId) || selectedFaces.find(fId) == selectedFaces.end()) {
            continue;
        }

        if (meshIsBorderFace(mesh, fId, ffAdj)) {
            borderFaces.push_back(fId);
        }

        return borderFaces;
    }

}

template<class Mesh, class Set>
std::vector<typename Mesh::VertexId> meshSubsetBorderVertices(
        const Mesh& mesh,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetBorderVertices(mesh, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
std::vector<typename Mesh::VertexId> meshSubsetBorderVertices(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;

    std::unordered_set<VertexId> borderVertices;

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId) || selectedFaces.find(fId) == selectedFaces.end()) {
            continue;
        }

        const Face& face = mesh.face(fId);

        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            if (meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj) || selectedFaces.find(ffAdj[fId][fePos]) == selectedFaces.end()) {
                borderVertices.insert(face.vertexId(fePos));
                borderVertices.insert(face.nextVertexId(fePos));
            }
        }
    }

    return std::vector<VertexId>(borderVertices.begin(), borderVertices.end());
}

template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::VertexId>> meshSubsetBorderVertexChains(
        const Mesh& mesh,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshBorderVertexChains(mesh, ffAdj, selectedFaces);
}

template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::VertexId>> meshSubsetBorderVertexChains(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;

    std::vector<std::vector<VertexId>> chains;

    std::vector<std::vector<VertexId>> nextMap(mesh.nextVertexId());

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId) || selectedFaces.find(fId) == selectedFaces.end()) {
            continue;
        }

        const Face& face = mesh.face(fId);

        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            if (meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj) || selectedFaces.find(ffAdj[fId][fePos]) == selectedFaces.end()) {
                VertexId vId = face.vertexId(fePos);
                VertexId nextVId = face.nextVertexId(fePos);
                nextMap[vId].push_back(nextVId);
            }
        }
    }

    std::vector<std::vector<bool>> visited(mesh.nextVertexId());
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId)) {
            continue;
        }

        visited[vId].resize(nextMap[vId].size(), false);
    }

    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId)) {
            continue;
        }

        //If it is a border vertex
        if (!nextMap[vId].empty()) {
            std::vector<VertexId> chain = internal::findBorderChainLoop(vId, vId, nextMap, visited);

            if (!chain.empty()) {
                chains.push_back(chain);
            }
        }
    }

    return chains;
}



namespace internal {

template<class VertexId>
std::vector<VertexId> findBorderChainLoop(
        const VertexId startId,
        const VertexId targetId,
        const std::vector<std::vector<VertexId>>& nextMap,
        std::vector<std::vector<bool>>& visited)
{
    std::vector<VertexId> chain;

    VertexId currentId = startId;

    bool first = true;
    while ((first || currentId != targetId) && nextMap[currentId].size() == 1) {
        if (visited[currentId][0]) {
            chain.clear();
            return chain;
        }

        chain.push_back(currentId);
        visited[currentId][0] = true;

        VertexId nextVertexId = nextMap[currentId][0];
        currentId = nextVertexId;

        first = false;
    }

    //Non-border!
    if (nextMap[currentId].empty()) {
        chain.clear();
        return chain;
    }

    if (first || currentId != targetId) {
        assert(nextMap[currentId].size() > 1);

        for (Index currentPath = 0; currentPath < nextMap[currentId].size(); ++currentPath) {
            if (visited[currentId][currentPath]) {
                continue;
            }

            std::vector<std::vector<bool>> tmpVisited = visited;
            tmpVisited[currentId][currentPath] = true;

            VertexId nextVertexId = nextMap[currentId][currentPath];
            std::vector<VertexId> tmpChain = internal::findBorderChainLoop(nextVertexId, targetId, nextMap, tmpVisited);

            if (!tmpChain.empty()) {
                visited = tmpVisited;

                chain.push_back(currentId);
                chain.insert(chain.end(), tmpChain.begin(), tmpChain.end());

                return chain;
            }
        }

        chain.clear();
    }

    return chain;
}

}

}
