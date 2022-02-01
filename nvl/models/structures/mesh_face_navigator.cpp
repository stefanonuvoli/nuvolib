/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_face_navigator.h"

namespace nvl {

template<class Mesh>
MeshFaceNavigator<Mesh>::MeshFaceNavigator(const Mesh& mesh) : vMesh(mesh)
{
    vFFAdj = meshFaceFaceAdjacencies(mesh, vFFEdgePos);
}

template<class Mesh>
MeshFaceNavigator<Mesh>::MeshFaceNavigator(
        const Mesh& mesh,
        const std::vector<std::vector<FaceId>>& ffAdj,
        const std::vector<std::vector<Index>>& ffEdgePos)
    : vMesh(mesh), vFFAdj(ffAdj), vFFEdgePos(ffEdgePos)
{
    vFId = NULL_ID;
    vEId = NULL_ID;
    vVertexFlipped = false;
}

template<class Mesh>
void MeshFaceNavigator<Mesh>::setFace(const FaceId& fId)
{
    this->vFId = fId;
}

template<class Mesh>
void MeshFaceNavigator<Mesh>::setEdge(const Index& eId)
{
    this->vEId = eId;
}

template<class Mesh>
void MeshFaceNavigator<Mesh>::setVertexFlipped(const bool flipped)
{
    this->vVertexFlipped = flipped;
}

template<class Mesh>
const typename MeshFaceNavigator<Mesh>::FaceId& MeshFaceNavigator<Mesh>::face() const
{
    assert(vFId != NULL_ID && vEId != NULL_ID);
    return this->vFId;
}

template<class Mesh>
const Index& MeshFaceNavigator<Mesh>::edge() const
{
    assert(vFId != NULL_ID && vEId != NULL_ID);
    return this->vEId;
}

template<class Mesh>
const typename MeshFaceNavigator<Mesh>::VertexId& MeshFaceNavigator<Mesh>::vertex() const
{
    assert(vFId != NULL_ID && vEId != NULL_ID);
    if (vVertexFlipped) {
        return this->vMesh.face(this->vFId).vertexId(this->vEId);
    }
    else {
        return this->vMesh.face(this->vFId).nextVertexId(this->vEId);
    }
}

template<class Mesh>
const bool& MeshFaceNavigator<Mesh>::isVertexFlipped() const
{
    assert(vFId != NULL_ID && vEId != NULL_ID);
    return this->vVertexFlipped;
}

template<class Mesh>
bool MeshFaceNavigator<Mesh>::flipFace()
{
    typedef typename Mesh::Face Face;

    if (vFId == NULL_ID || vEId == NULL_ID)
        return false;

    const Face& face = vMesh.face(vFId);

    for (Index fePos = 0; fePos < face.vertexNumber(); ++fePos) {
        if (vFFEdgePos[vFId] == vEId) {
            FaceId lastFId = vFId;
            vFId = vFFAdj[fePos];

            const Face& adjFace = vMesh.face(vFId);
            for (Index adjPos = 0; adjPos < adjFace.vertexNumber(); ++adjPos) {
                if (vFFAdj[adjPos] == vFId) {
                    vEId = adjPos;
                }
            }
        }
    }

    assert(meshIsBorderFaceEdge(vMesh, vFId, vEId, vFFEdgePos));

    return false;
}

template<class Mesh>
bool MeshFaceNavigator<Mesh>::flipVertex()
{
    if (vFId == NULL_ID || vEId == NULL_ID)
        return false;

    vVertexFlipped = true;

    return true;
}

template<class Mesh>
bool MeshFaceNavigator<Mesh>::nextEdge()
{
    typedef typename Mesh::Face Face;
    if (vFId == NULL_ID || vEId == NULL_ID)
        return false;

    const Face& face = vMesh.face(vFId);
    vEId = (vEId + 1) % face.vertexNumber();
}

template<class Mesh>
bool MeshFaceNavigator<Mesh>::prevEdge()
{
    typedef typename Mesh::Face Face;
    if (vFId == NULL_ID || vEId == NULL_ID)
        return false;

    const Face& face = vMesh.face(vFId);

    if (vEId == 0)
        vEId = face.vertexNumber() - 1;
    else
        --vEId;
}

template<class Mesh>
bool MeshFaceNavigator<Mesh>::isOnBorder() const
{
    return meshIsBorderFaceEdge(vMesh, vFId, vEId, vFFEdgePos);
}

}
