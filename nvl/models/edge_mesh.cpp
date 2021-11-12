#include "edge_mesh.h"

namespace nvl {

template<class V, class L, class F, class E>
EdgeMesh<V,L,F,E>::EdgeMesh()
{
    this->vType = MeshType::EDGE;
}

template<class V, class L, class F, class E>
void EdgeMesh<V,L,F,E>::compactAll()
{
    compactVertices();
    this->compactPolylines();
    compactFaces();
    compactEdges();
}

template<class V, class L, class F, class E>
std::vector<typename EdgeMesh<V,L,F,E>::VertexId> EdgeMesh<V,L,F,E>::compactVertices()
{
    std::vector<VertexId> vMap = FaceMesh<V,L,F>::compactVertices();

    //Update vertex ids in edges
    for (Edge& edge : this->edges()) {
        VertexId oldId;
        oldId = edge.firstVertexId();
        if (oldId != MAX_INDEX) {
            VertexId newId = vMap.at(oldId);
            edge.setFirstVertexId(newId);
        }
        oldId = edge.secondVertexId();
        if (oldId != MAX_INDEX) {
            VertexId newId = vMap.at(oldId);
            edge.setSecondVertexId(newId);
        }
    }

    return vMap;
}

template<class V, class L, class F, class E>
std::vector<typename EdgeMesh<V,L,F,E>::FaceId> EdgeMesh<V,L,F,E>::compactFaces()
{
    std::vector<FaceId> fMap = FaceMesh<V,L,F>::compactFaces();

    //Update face ids in edges
    for (Edge& edge : this->edges()) {
        FaceId oldId = edge.faceId();
        if (oldId != MAX_INDEX) {
            FaceId newId = fMap.at(oldId);
            edge.setFaceId(newId);
        }
    }

    return fMap;
}


template<class V, class L, class F, class E>
std::vector<typename EdgeMesh<V,L,F,E>::EdgeId> EdgeMesh<V,L,F,E>::compactEdges()
{
    std::vector<Index> eMap = this->vEdges.compact();

    //Update face ids
    FaceId newEdgeId = 0;
    for (Edge& edge : this->edges()) {
        edge.setId(newEdgeId);
        ++newEdgeId;
    }

    return eMap;
}

template<class V, class L, class F, class E>
void EdgeMesh<V,L,F,E>::clear()
{
    FaceMesh<V,L,F>::clear();
    this->clearEdges();
}

template<class V, class L, class F, class E>
std::ostream& operator<<(std::ostream& output, const EdgeMesh<V,L,F,E>& edgeMesh)
{
    const FaceMesh<V,L,F>& sMesh = edgeMesh;
    output << sMesh;

    output << "Edges: " << edgeMesh.edgeNumber() << std::endl;
    for (const typename EdgeMesh<V,L,F,E>::Edge& edge : edgeMesh.edges()) {
        output << edge << std::endl;
    }

    return output;
}

}
