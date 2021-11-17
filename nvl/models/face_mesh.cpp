#include "face_mesh.h"

#include <nvl/models/mesh_normals.h>

namespace nvl {

template<class V, class L, class F>
FaceMesh<V,L,F>::FaceMesh() :
    vFaceNormalsEnabled(false),
    vFaceMaterialEnabled(false),
    vWedgeNormalsEnabled(false),
    vWedgeUVsEnabled(false)
{
    this->vType = MeshType::FACE;
}

template<class V, class L, class F>
typename FaceMesh<V,L,F>::FaceId FaceMesh<V,L,F>::addFace(const Face& face)
{
    FaceId newFId = MeshFaceHandler<F>::addFace(face);
    updateComponents();
    return newFId;
}

template<class V, class L, class F>
template<class... Ts>
typename FaceMesh<V,L,F>::FaceId FaceMesh<V,L,F>::addFace(const Ts... vertices)
{
    FaceId newFId = MeshFaceHandler<F>::addFace(vertices...);
    updateComponents();
    return newFId;
}

template<class V, class L, class F>
typename FaceMesh<V,L,F>::FaceId FaceMesh<V,L,F>::allocateFaces(const Size& n)
{
    return allocateFaces(n, Face());
}

template<class V, class L, class F>
typename FaceMesh<V,L,F>::FaceId FaceMesh<V,L,F>::allocateFaces(const Size& n, const Face& face)
{
    FaceId newFId = MeshFaceHandler<F>::allocateFaces(n, face);
    updateComponents();
    return newFId;
}

template<class V, class L, class F>
typename F::VertexContainer& FaceMesh<V,L,F>::faceVertexIds(const FaceId& id)
{
    return MeshFaceHandler<F>::face(id).vertexIds();
}

template<class V, class L, class F>
const typename F::VertexContainer& FaceMesh<V,L,F>::faceVertexIds(const FaceId& id) const
{
    return MeshFaceHandler<F>::face(id).vertexIds();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::setFaceVertexIds(const FaceId& id, const typename F::VertexContainer& vertexIds)
{
    MeshFaceHandler<F>::face(id).setVertexIds(vertexIds);
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
    meshComputeFaceNormalsSVDFitting(*this);
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::computeVertexNormals()
{
    meshComputeVertexNormalsFromFaceNormals(*this);
}

template<class V, class L, class F>
bool FaceMesh<V,L,F>::hasFaceNormals() const
{
    return vFaceNormalsEnabled;
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::enableFaceNormals()
{
    vFaceNormalsEnabled = true;
    updateFaceNormals();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::disableFaceNormals()
{
    vFaceNormalsEnabled = false;
    updateFaceNormals();
}

template<class V, class L, class F>
bool FaceMesh<V,L,F>::hasFaceMaterials() const
{
    return vFaceMaterialEnabled;
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::enableFaceMaterials()
{
    vFaceMaterialEnabled = true;
    updateFaceMaterials();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::disableFaceMaterials()
{
    vFaceMaterialEnabled = false;
    updateFaceMaterials();
}

template<class V, class L, class F>
bool FaceMesh<V,L,F>::hasWedgeNormals() const
{
    return vWedgeNormalsEnabled;
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::enableWedgeNormals()
{
    vWedgeNormalsEnabled = true;
    updateFaceWedgeNormals();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::disableWedgeNormals()
{
    vWedgeNormalsEnabled = false;
    updateFaceWedgeNormals();
}

template<class V, class L, class F>
bool FaceMesh<V,L,F>::hasWedgeUVs() const
{
    return vWedgeUVsEnabled;
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::enableWedgeUVs()
{
    vWedgeUVsEnabled = true;
    updateFaceWedgeUVs();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::disableWedgeUVs()
{
    vWedgeUVsEnabled = false;
    updateFaceWedgeUVs();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::compactAll()
{
    compactVertices();
    this->compactFaces();
    compactFaces();
    compactMaterials();
    compactWedgeNormals();
    compactWedgeUVs();
}

template<class V, class L, class F>
std::vector<typename FaceMesh<V,L,F>::MaterialId> FaceMesh<V,L,F>::compactMaterials()
{
    std::vector<MaterialId> mMap = this->vMaterials.compact();

    //Update vertex ids in faces
    if (this->hasFaceMaterials()) {
        for (Face& face : this->faces()) {
            MaterialId oldId = this->faceMaterialId(face);
            if (oldId != NULL_ID) {
                MaterialId newId = mMap.at(oldId);
                this->setFaceMaterialId(face, newId);
            }
        }
    }

    return mMap;
}

template<class V, class L, class F>
std::vector<typename FaceMesh<V,L,F>::WedgeNormalId> FaceMesh<V,L,F>::compactWedgeNormals()
{
    std::vector<WedgeNormalId> wMap = this->vWedgeUVs.compact();

    //Update vertex ids in faces
    if (this->hasWedgeUVs()) {
        for (Face& face : this->faces()) {
            std::vector<WedgeNormalId> oldIds = this->faceWedgeUVs(face);
            std::vector<WedgeNormalId> newIds(oldIds.size(), nvl::NULL_ID);
            for (Index i = 0; i < oldIds.size(); ++i) {
                const WedgeNormalId& id = oldIds[i];
                if (id != NULL_ID) {
                    WedgeNormalId newId = wMap.at(id);
                    newIds[i] = newId;
                }
            }
            this->setFaceWedgeUVs(face, newIds);
        }
    }

    return wMap;
}

template<class V, class L, class F>
std::vector<typename FaceMesh<V,L,F>::WedgeUVId> FaceMesh<V,L,F>::compactWedgeUVs()
{
    std::vector<WedgeUVId> wMap = this->vWedgeUVs.compact();

    //Update vertex ids in faces
    if (this->hasWedgeUVs()) {
        for (Face& face : this->faces()) {
            std::vector<WedgeUVId> oldIds = this->faceWedgeUVs(face);
            std::vector<WedgeUVId> newIds(oldIds.size(), nvl::NULL_ID);
            for (Index i = 0; i < oldIds.size(); ++i) {
                const WedgeUVId& id = oldIds[i];
                if (id != NULL_ID) {
                    WedgeUVId newId = wMap.at(id);
                    newIds[i] = newId;
                }
            }
            this->setFaceWedgeUVs(face, newIds);
        }
    }

    return wMap;
}

template<class V, class L, class F>
std::vector<typename FaceMesh<V,L,F>::FaceId> FaceMesh<V,L,F>::compactFaces()
{
    //Compact face materials
    if (this->hasFaceMaterials()) {
        for (FaceId fId = 0; fId < this->nextFaceId(); fId++) {
            if (this->isFaceDeleted(fId)) {
                this->vFaceMaterials.erase(fId);
            }
        }
        this->vFaceMaterials.compact();
    }
    else {
        this->vFaceMaterials.clear();
    }

    //Compact face normals
    if (this->hasFaceNormals()) {
        for (FaceId fId = 0; fId < this->nextFaceId(); fId++) {
            if (this->isFaceDeleted(fId)) {
                this->vFaceNormals.erase(fId);
            }
        }
        this->vFaceNormals.compact();
    }
    else {
        this->vFaceNormals.clear();
    }

    //Compact face normals
    if (this->hasWedgeNormals()) {
        for (FaceId fId = 0; fId < this->nextFaceId(); fId++) {
            if (this->isFaceDeleted(fId)) {
                this->vFaceWedgeNormals.erase(fId);
            }
        }
        this->vFaceWedgeNormals.compact();
    }
    else {
        this->vFaceWedgeNormals.clear();
    }

    //Compact face normals
    if (this->hasWedgeUVs()) {
        for (FaceId fId = 0; fId < this->nextFaceId(); fId++) {
            if (this->isFaceDeleted(fId)) {
                this->vFaceWedgeUVs.erase(fId);
            }
        }
        this->vFaceWedgeUVs.compact();
    }
    else {
        this->vFaceWedgeUVs.clear();
    }

    //Compact faces
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
std::vector<typename FaceMesh<V,L,F>::VertexId> FaceMesh<V,L,F>::compactVertices()
{
    std::vector<VertexId> vMap = PolylineMesh<V,L>::compactVertices();

    //Update vertex ids in faces
    for (Face& face : this->faces()) {
        for (Index i = 0; i < face.vertexNumber(); ++i) {
            VertexId oldId = face.vertexId(i);
            if (oldId != NULL_ID) {
                VertexId newId = vMap.at(oldId);
                face.setVertexId(i, newId);
            }
        }
    }

    return vMap;
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::clearFaces()
{
    MeshFaceHandler<F>::clearFaces();
    this->vFaceMaterials.clear();
    this->vFaceNormals.clear();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::clear()
{
    PolylineMesh<V,L>::clear();
    this->clearFaces();
    this->clearMaterials();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::updateComponents()
{
    updateFaceMaterials();
    updateFaceNormals();
    updateFaceWedgeNormals();
    updateFaceWedgeUVs();
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::updateFaceMaterials()
{
    if (vFaceMaterialEnabled) {
        this->vFaceMaterials.resize(this->nextFaceId(), NULL_ID);
    }
    else {
        this->vFaceMaterials.clear();
    }
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::updateFaceNormals()
{
    if (vFaceNormalsEnabled) {
        this->vFaceNormals.resize(this->nextFaceId(), FaceNormal::Zero());
    }
    else {
        this->vFaceNormals.clear();
    }
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::updateFaceWedgeNormals()
{
    if (vWedgeNormalsEnabled) {
        this->vFaceWedgeNormals.resize(this->nextFaceId());
    }
    else {
        this->vFaceWedgeNormals.clear();
    }
}

template<class V, class L, class F>
void FaceMesh<V,L,F>::updateFaceWedgeUVs()
{
    if (vWedgeUVsEnabled) {
        this->vFaceWedgeUVs.resize(this->nextFaceId());
    }
    else {
        this->vFaceWedgeUVs.clear();
    }
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
