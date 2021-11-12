#include "mesh_adjacencies.h"

#include <stack>
#include <unordered_set>

namespace nvl {

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

template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshVertexFaceAdjacencies(
        const Mesh& mesh)
{
    std::vector<std::vector<Index>> vfPos;
    return meshVertexFaceAdjacencies(mesh, vfPos);
}

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

template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshFaceFaceAdjacencies(
        const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::VertexId>> vfAdj = meshVertexFaceAdjacencies(mesh);
    return meshFaceFaceAdjacencies(mesh, vfAdj);
}

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

        ffAdj[fId].resize(face.vertexNumber(), MAX_INDEX);

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

template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshConnectedComponents(mesh, ffAdj);
}

template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    std::vector<Index> faceComponentMap;
    return meshConnectedComponents(mesh, ffAdj, faceComponentMap);
}

template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh,
        std::vector<Index>& faceComponentMap)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshConnectedComponents(mesh, ffAdj, faceComponentMap);
}

template<class Mesh>
std::vector<std::vector<typename Mesh::FaceId>> meshConnectedComponents(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        std::vector<Index>& faceComponentMap)
{
    typedef typename Mesh::FaceId FaceId;

    std::vector<std::vector<typename Mesh::FaceId>> connectedComponents;
    faceComponentMap.resize(mesh.nextFaceId(), MAX_INDEX);

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
                    if (adjId != MAX_INDEX && !visited[adjId]) {
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


template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetConnectedComponents(mesh, selectedFaces, ffAdj);
}

template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj)
{
    std::vector<Index> faceComponentMap;
    return meshSubsetConnectedComponents(mesh, selectedFaces, ffAdj, faceComponentMap);
}

template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces,
        std::vector<Index>& faceComponentMap)
{
    std::vector<std::vector<typename Mesh::FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);
    return meshSubsetConnectedComponents(mesh, selectedFaces, ffAdj, faceComponentMap);
}

template<class Mesh, class Set>
std::vector<std::vector<typename Mesh::FaceId>> meshSubsetConnectedComponents(
        const Mesh& mesh,
        const Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        std::vector<Index>& faceComponentMap)
{
    typedef typename Mesh::FaceId FaceId;

    std::vector<std::vector<typename Mesh::FaceId>> connectedComponents;
    faceComponentMap.resize(mesh.nextFaceId(), MAX_INDEX);

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
                    if (adjId != MAX_INDEX && !visited[adjId]) {
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
