#include "face_mesh.h"

#include <nvl/models/mesh_normals.h>

namespace nvl {

template<class V, class L, class F>
FaceMesh<V,L,F>::FaceMesh()
{
    this->vType = MeshType::FACE;
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::compactAll()
{
    compactVertices();
    this->compactPolylines();
    compactFaces();
    compactMaterials();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::computeNormals()
{
    computeFaceNormals();
    computeVertexNormals();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::computeFaceNormals()
{
    nvl::meshComputeFaceNormalsSVDFitting(*this);
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::computeVertexNormals()
{
    nvl::meshComputeVertexNormalsFromFaceNormals(*this);
}

template<class V, class L, class F>
std::vector<typename FaceMesh<V,L,F>::VertexId> FaceMesh<V,L,F>::compactVertices()
{
    std::vector<VertexId> vMap = PolylineMesh<V,L>::compactVertices();

    //Update vertex ids in faces
    for (Face& face : this->faces()) {
        for (Index i = 0; i < face.vertexNumber(); ++i) {
            VertexId oldId = face.vertexId(i);
            if (oldId != MAX_INDEX) {
                VertexId newId = vMap.at(oldId);
                face.setVertexId(i, newId);
            }
        }
    }

    return vMap;
}

template<class V, class L, class F>
std::vector<typename FaceMesh<V,L,F>::MaterialId> FaceMesh<V,L,F>::compactMaterials()
{
    std::vector<Index> mMap = this->vMaterials.compact();

    //Update vertex ids in faces
    for (Face& face : this->faces()) {
        MaterialId oldId = face.materialId();
        if (oldId != MAX_INDEX) {
            VertexId newId = mMap.at(oldId);
            face.setMaterialId(newId);
        }
    }

    return mMap;
}

template<class V, class L, class F>
std::vector<typename FaceMesh<V,L,F>::FaceId> FaceMesh<V,L,F>::compactFaces()
{
    std::vector<Index> fMap = this->vFaces.compact();

    //Update face ids
    FaceId newFaceId = 0;
    for (Face& face : this->faces()) {
        face.setId(newFaceId);
        ++newFaceId;
    }

    return fMap;
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::clear()
{
    PolylineMesh<V,L>::clear();
    this->clearFaces();
    this->clearMaterials();
}

template<class V, class L, class F>
std::ostream& operator<<(std::ostream& output, const FaceMesh<V,L,F>& faceMesh)
{
    const PolylineMesh<V,L>& eMesh = faceMesh;
    output << eMesh;

    output << "Faces: " << faceMesh.faceNumber() << std::endl;
    for (const typename FaceMesh<V,L,F>::Face& face : faceMesh.faces()) {
        output << face << std::endl;
    }

    return output;
}

}
