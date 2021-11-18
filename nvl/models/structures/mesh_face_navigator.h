#ifndef NVL_MODELS_MESH_FACE_NAVIGATOR_H
#define NVL_MODELS_MESH_FACE_NAVIGATOR_H

#include <nvl/nuvolib.h>

#include <nvl/models/algorithms/mesh_adjacencies.h>
#include <nvl/models/algorithms/mesh_borders.h>

#include <vector>

namespace nvl {

template<class Mesh>
class MeshFaceNavigator {
public:

    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    MeshFaceNavigator(const Mesh& vMesh);
    MeshFaceNavigator(
            const Mesh& vMesh,
            const std::vector<std::vector<FaceId>>& vFFAdj,
            const std::vector<std::vector<Index>>& vFFEdgePos);

    void setFace(const FaceId& vFId);
    void setEdge(const Index& vEId);
    void setVertexFlipped(const bool flipped);

    const FaceId& face() const;
    const Index& edge() const;
    const VertexId& vertex() const;
    const bool& isVertexFlipped() const;


    bool flipFace();
    bool flipVertex();
    bool nextEdge();
    bool prevEdge();

    bool isOnBorder() const;

private:

    FaceId vFId;
    Index vEId;
    bool vVertexFlipped;

    const Mesh& vMesh;
    std::vector<std::vector<FaceId>> vFFAdj;
    std::vector<std::vector<Index>> vFFEdgePos;

};

}

#include "mesh_face_navigator.cpp"

#endif // NVL_MODELS_MESH_FACE_NAVIGATOR_H
