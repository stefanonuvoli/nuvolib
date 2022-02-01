/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_adjacencies.h"

#include <stack>
#include <unordered_set>

namespace nvl {

/**
 * @brief Mesh vertex-vertex adjacencies
 * @param mesh Mesh
 * @param faces Flag for taking into account face adjacencies (default true)
 * @param polylines Flag for taking into account polyline adjacencies (default true)
 * @return Vertex-vertex adjacencies
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::VertexId>> meshVertexVertexAdjacencies(
        const Mesh& mesh,
        const bool& faces,
        const bool& polylines)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::Polyline Polyline;
    typedef typename Mesh::VertexId VertexId;

    std::vector<std::vector<typename Mesh::VertexId>> vvAdj;

    vvAdj.resize(mesh.nextVertexId());

    if (faces) {
        for (const Face& face : mesh.faces()) {
            for (Index i = 0; i < face.vertexIds().size(); ++i) {
                const VertexId& v1 = face.vertexId(i);
                const VertexId& v2 = face.nextVertexId(i);

                vvAdj[v1].push_back(v2);
                vvAdj[v2].push_back(v1);
            }
        }
    }

    if (polylines) {
        for (const Polyline& polyline : mesh.polylines()) {
            for (Index i = 0; i < polyline.vertexIds().size() - 1; ++i) {
                const VertexId& v1 = polyline.vertexId(i);
                const VertexId& v2 = polyline.nextVertexId(i);

                vvAdj[v1].push_back(v2);
                vvAdj[v2].push_back(v1);
            }
        }
    }

    for (Index i = 0; i < vvAdj.size(); ++i) {
        //Remove duplicates
        std::sort(vvAdj[i].begin(), vvAdj[i].end());
        vvAdj[i].erase(std::unique(vvAdj[i].begin(), vvAdj[i].end()), vvAdj[i].end());
    }

    return vvAdj;
}

/**
 * @brief Mesh vertex-face adjacencies
 * @param mesh Mesh
 * @return Vertex-face adjacencies
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshVertexFaceAdjacencies(
        const Mesh& mesh)
{
    std::vector<std::vector<Index>> vfPos;
    return meshVertexFaceAdjacencies(mesh, vfPos);
}

/**
 * @brief Mesh vertex-face adjacencies
 * @param mesh Mesh
 * @param vfPos Output vector that gives the position of the vertex in the adjacent face
 * @return Vertex-face adjacencies
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshVertexFaceAdjacencies(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& vfPos)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::VertexId VertexId;

    std::vector<std::vector<typename Mesh::VertexId>> vfAdj;

    vfAdj.resize(mesh.nextVertexId());
    vfPos.resize(mesh.nextVertexId());

    for (const Face& face : mesh.faces()) {
        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            const VertexId& vId = face.vertexId(fePos);

            vfAdj[vId].push_back(face.id());
            vfPos[vId].push_back(fePos);
        }
    }

    return vfAdj;
}

/**
 * @brief Mesh face-face adjacencies
 * @param mesh Mesh
 * @return Face-face adjacencies
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshFaceFaceAdjacencies(
        const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::VertexId>> vfAdj = meshVertexFaceAdjacencies(mesh);
    return meshFaceFaceAdjacencies(mesh, vfAdj);
}


/**
 * @brief Mesh face-face adjacencies
 * @param mesh Mesh
 * @param vfAdj Pre-computed vertex-vertex adjacencies
 * @return Face-face adjacencies
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshFaceFaceAdjacencies(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vfAdj)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    std::vector<std::vector<typename Mesh::FaceId>> ffAdj;

    ffAdj.resize(mesh.nextFaceId());

    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;

    #pragma omp parallel for
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId)) {
            continue;
        }

        const Face& face = mesh.face(fId);

        ffAdj[fId].resize(face.vertexNumber(), NULL_ID);

        for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
            const VertexId& vId = face.vertexId(fePos);
            const VertexId& nextVId = face.nextVertexId(fePos);

            for (const FaceId& adjFId : vfAdj[vId]) {
                if (adjFId == fId)
                    continue;

                const Face& adjFace = mesh.face(adjFId);

                for (Index adjPos = 0; adjPos < adjFace.vertexNumber(); ++adjPos) {
                    const VertexId& adjVId = adjFace.vertexId(adjPos);
                    const VertexId& adjNextVId = adjFace.nextVertexId(adjPos);

                    if (adjVId == nextVId && adjNextVId == vId) {
                        ffAdj[fId][fePos] = adjFId;
                    }
                }
            }
        }
    }

    return ffAdj;
}

/**
 * @brief Connected components of a mesh
 * @param mesh Mesh
 * @return Connected components as a vector of face ids
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshConnectedComponents(mesh, ffAdj);
}

/**
 * @brief Connected components of a mesh
 * @param mesh Mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @return Connected components as a vector of face ids
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    std::vector<Index> faceComponentMap;
    return meshConnectedComponents(mesh, ffAdj, faceComponentMap);
}

/**
 * @brief Connected components of a mesh
 * @param mesh Mesh
 * @param faceComponentMap Map that maps to each face the component it belongs in
 * @return Connected components as a vector of face ids
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh,
        std::vector<Index>& faceComponentMap)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshConnectedComponents(mesh, ffAdj, faceComponentMap);
}

/**
 * @brief Connected components of a mesh
 * @param mesh Mesh
 * @param ffAdj Pre-computed face-face adjacencies
 * @param faceComponentMap Map that maps to each face the component it belongs in
 * @return Connected components as a vector of face ids
 */
template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        std::vector<Index>& faceComponentMap)
{
    typedef typename Mesh::FaceId FaceId;

    std::vector<std::vector<typename Mesh::FaceId>> connectedComponents;
    faceComponentMap.resize(mesh.nextFaceId(), NULL_ID);

    std::stack<FaceId> stack;
    std::vector<bool> visited(mesh.nextFaceId(), false);
    for (FaceId i = 0; i < mesh.nextFaceId(); ++i) {
        if (visited[i] || mesh.isFaceDeleted(i)) {
            continue;
        }

        stack.push(i);

        std::vector<FaceId> currentComponent;

        while (!stack.empty()) {
            FaceId fId = stack.top();
            stack.pop();

            if (!visited[fId]) {
                currentComponent.push_back(fId);
                faceComponentMap[fId] = connectedComponents.size();

                for (const FaceId& adjId : ffAdj[fId]) {
                    if (adjId != NULL_ID && !visited[adjId]) {
                        stack.push(adjId);
                    }
                }

                visited[fId] = true;
            }
        }

        connectedComponents.push_back(currentComponent);
    }

    return connectedComponents;
}

/**
 * @brief Connected components of a mesh
 * @param mesh Mesh
 * @param selectedFaces Face subset on which compute the connected components
 * @return Connected components as a vector of face ids
 */
template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetConnectedComponents(mesh, selectedFaces, ffAdj);
}

/**
 * @brief Connected components of a mesh
 * @param mesh Mesh
 * @param selectedFaces Face subset on which compute the connected components
 * @param ffAdj Pre-computed face-face adjacencies
 * @return Connected components as a vector of face ids
 */
template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    std::vector<Index> faceComponentMap;
    return meshSubsetConnectedComponents(mesh, selectedFaces, ffAdj, faceComponentMap);
}

/**
 * @brief Connected components of a mesh
 * @param mesh Mesh
 * @param selectedFaces Face subset on which compute the connected components
 * @param faceComponentMap Map that maps to each face the component it belongs in
 * @return Connected components as a vector of face ids
 */
template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces,
        std::vector<Index>& faceComponentMap)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetConnectedComponents(mesh, selectedFaces, ffAdj, faceComponentMap);
}

/**
 * @brief Connected components of a mesh
 * @param mesh Mesh
 * @param selectedFaces Face subset on which compute the connected components
 * @param ffAdj Pre-computed face-face adjacencies
 * @param faceComponentMap Map that maps to each face the component it belongs in
 * @return Connected components as a vector of face ids
 */
template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        std::vector<Index>& faceComponentMap)
{
    typedef typename Mesh::FaceId FaceId;

    std::vector<std::vector<typename Mesh::FaceId>> connectedComponents;
    faceComponentMap.resize(mesh.nextFaceId(), NULL_ID);

    std::stack<FaceId> stack;
    std::vector<bool> visited(mesh.nextFaceId(), false);
    for (FaceId i = 0; i < mesh.nextFaceId(); ++i) {
        if (mesh.isFaceDeleted(i) || visited[i] || selectedFaces.find(i) == selectedFaces.end()) {
            continue;
        }

        stack.push(i);

        std::vector<FaceId> currentComponent;

        while (!stack.empty()) {
            FaceId fId = stack.top();
            stack.pop();

            if (!visited[fId] && selectedFaces.find(fId) != selectedFaces.end()) {
                currentComponent.push_back(fId);
                faceComponentMap[fId] = connectedComponents.size();

                for (FaceId adjId : ffAdj[fId]) {
                    if (adjId != NULL_ID && !visited[adjId]) {
                        stack.push(adjId);
                    }
                }

                visited[fId] = true;
            }
        }

        connectedComponents.push_back(currentComponent);
    }

    return connectedComponents;
}

}
