/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_borders.h"

#include <unordered_set>

namespace nvl {

namespace internal {

template<class VertexId>
std::vector<VertexId> findBorderVertexChainPath(
        const VertexId& startId,
        const VertexId& targetId,
        const std::vector<std::vector<VertexId>>& nextMap,
        std::vector<std::vector<bool>>& visited);

}

/**
 * @brief Check if a face edge is on the borders of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @param fePos Id of the edge of the face
 * @return True if the edge is a border
 */
template<class Mesh>
bool meshIsBorderFaceEdge(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Index& fePos)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshIsBorderFaceEdge(mesh, fId, fePos, ffAdj);
}

/**
 * @brief Check if a face edge is on the borders of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @param fePos Id of the edge of the face
 * @param ffAdj Pre-computed face-face adjacencies
 * @return True if the edge is a border
 */
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

/**
 * @brief Check if a face is on the borders of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @param fePos Id of the edge of the face
 * @return True if the face has a border edge
 */
template<class Mesh>
bool meshIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshIsBorderFace(mesh, fId, ffAdj);
}

/**
 * @brief Check if a face is on the borders of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @param ffAdj Pre-computed face-face adjacencies
 * @return True if the face has a border edge
 */
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

/**
 * @brief Check if a vertex is on the borders of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @return True if the vertex is on the border
 */
template<class Mesh>
bool meshIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshIsBorderVertex(mesh, vId, ffAdj);
}

/**
 * @brief Check if a vertex is on the borders of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @param ffAdj Pre-computed face-face adjacencies
 * @return True if the vertex is on the border
 */
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

/**
 * @brief Get all faces lying on a border in a mesh
 * @param mesh Mesh
 * @return Faces lying on a border
 */
template<class Mesh>
std::vector<typename Mesh::FaceId> meshBorderFaces(const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshBorderFaces(mesh, ffAdj);
}

/**
 * @brief Get all faces lying on a border in a mesh
 * @param mesh Mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return Faces lying on a border
 */
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

/**
 * @brief Get all vertices lying on a border in a mesh
 * @param mesh Mesh
 * @return Vertices lying on a border
 */
template<class Mesh>
std::vector<typename Mesh::VertexId> meshBorderVertices(const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshBorderVertices(mesh, ffAdj);
}

/**
 * @brief Get all vertices lying on a border in a mesh
 * @param mesh Mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return Vertices lying on a border
 */
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

/**
 * @brief Get all vertex chains on a border in a mesh
 * @param mesh Mesh
 * @return Vertex chains lying on a border
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::VertexId>> meshBorderVertexChains(
        const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshBorderVertexChains(mesh, ffAdj);
}

/**
 * @brief Get all vertex chains on a border in a mesh
 * @param mesh Mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return Vertex chains lying on a border
 */
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
            std::vector<VertexId> chain = internal::findBorderVertexChainPath(vId, vId, nextMap, visited);

            if (!chain.empty()) {
                chains.push_back(chain);
            }
        }
    }

    return chains;
}

/**
 * @brief Check if a face edge is on the borders of a subset of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @param fePos Id of the edge of the face
 * @param selectedFaces Faces in the subset of the mesh
 * @return True if the edge is a border
 */
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

/**
 * @brief Check if a face edge is on the borders of a subset of mesh
 * @param mesh Mesh
 * @param fId Face id
 * @param fePos Id of the edge of the face
 * @param selectedFaces Faces in the subset of the mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return True if the edge is a border
 */
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

/**
 * @brief Check if a face is on the borders of a subset of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @param fePos Id of the edge of the face
 * @param selectedFaces Faces in the subset of the mesh
 * @return True if the face has a border edge
 */
template<class Mesh, class Set>
bool meshSubsetIsBorderFace(
        const Mesh& mesh,
        const typename Mesh::FaceId& fId,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetIsBorderFace(mesh, fId, selectedFaces, ffAdj);
}

/**
 * @brief Check if a face is on the borders of a subset of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @param fePos Id of the edge of the face
 * @param selectedFaces Faces in the subset of the mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return True if the face has a border edge
 */
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

/**
 * @brief Check if a vertex is on the borders of a subset a subset of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @param selectedFaces Faces in the subset of the mesh
 * @return True if the vertex is on the border
 */
template<class Mesh, class Set>
bool meshSubsetIsBorderVertex(
        const Mesh& mesh,
        const typename Mesh::VertexId& vId,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetIsBorderVertex(mesh, vId, selectedFaces, ffAdj);
}

/**
 * @brief Check if a vertex is on the borders of a subset of a mesh
 * @param mesh Mesh
 * @param vId Vertex Id
 * @param selectedFaces Faces in the subset of the mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return True if the vertex is on the border
 */
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

/**
 * @brief Get all faces lying on a border in a subset of a mesh
 * @param mesh Mesh
 * @param selectedFaces Faces in the subset of the mesh
 * @return Faces lying on a border
 */
template<class Mesh, class Set>
std::vector<typename Mesh::VertexId> meshSubsetBorderFaces(
        const Mesh& mesh,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetBorderFaces(mesh, selectedFaces, ffAdj);
}

/**
 * @brief Get all faces lying on a border in a subset of a mesh
 * @param mesh Mesh
 * @param selectedFaces Faces in the subset of the mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return Faces lying on a border
 */
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

/**
 * @brief Get all vertices lying on a border in a subset of a mesh
 * @param mesh Mesh
 * @param selectedFaces Faces in the subset of the mesh
 * @return Vertices lying on a border
 */
template<class Mesh, class Set>
std::vector<typename Mesh::VertexId> meshSubsetBorderVertices(
        const Mesh& mesh,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetBorderVertices(mesh, selectedFaces, ffAdj);
}

/**
 * @brief Get all vertices lying on a border in a subset of a mesh
 * @param mesh Mesh
 * @param selectedFaces Faces in the subset of the mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return Vertices lying on a border
 */
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

/**
 * @brief Get all vertex chains on a border in a subset of a mesh
 * @param mesh Mesh
 * @param selectedFaces Faces in the subset of the mesh
 * @return Vertex chains lying on a border
 */
template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::VertexId>> meshSubsetBorderVertexChains(
        const Mesh& mesh,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshBorderVertexChains(mesh, ffAdj, selectedFaces);
}

/**
 * @brief Get all vertex chains on a border in a subset of a mesh
 * @param mesh Mesh
 * @param selectedFaces Faces in the subset of the mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return Vertex chains lying on a border
 */
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
            std::vector<VertexId> chain = internal::findBorderVertexChainPath(vId, vId, nextMap, visited);

            if (!chain.empty()) {
                chains.push_back(chain);
            }
        }
    }

    return chains;
}



namespace internal {

/**
 * @brief Find the path of the vertex chain in the mesh, given starting and ending vertex
 * @param startId Starting vertex
 * @param targetId Ending vertex
 * @param nextMap Tmp map
 * @param visited Visited flag vector
 * @return Vertex chain connecting the vertices
 */
template<class VertexId>
std::vector<VertexId> findBorderVertexChainPath(
        const VertexId& startId,
        const VertexId& targetId,
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
            std::vector<VertexId> tmpChain = internal::findBorderVertexChainPath(nextVertexId, targetId, nextMap, tmpVisited);

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
