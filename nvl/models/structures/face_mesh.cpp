#include "face_mesh.h"

#include <nvl/models/algorithms/mesh_normals.h>

namespace nvl {

template<class VT, class PT, class FT>
FaceMesh<VT,PT,FT>::FaceMesh() :
    vFaceNormalsEnabled(false),
    vFaceMaterialEnabled(false),
    vWedgeNormalsEnabled(false),
    vWedgeUVsEnabled(false)
{
    this->vType = MeshType::FACE;
}

template<class VT, class PT, class FT>
typename FaceMesh<VT,PT,FT>::FaceId FaceMesh<VT,PT,FT>::addFace(const Face& face)
{
    FaceId newFId = MeshFaceHandler<Face>::addFace(face);
    updateComponents();
    return newFId;
}

template<class VT, class PT, class FT>
template<class... Ts>
typename FaceMesh<VT,PT,FT>::FaceId FaceMesh<VT,PT,FT>::addFace(const Ts... vertices)
{
    FaceId newFId = MeshFaceHandler<Face>::addFace(vertices...);
    updateComponents();
    return newFId;
}

template<class VT, class PT, class FT>
typename FaceMesh<VT,PT,FT>::FaceId FaceMesh<VT,PT,FT>::allocateFaces(const Size& n)
{
    return allocateFaces(n, Face());
}

template<class VT, class PT, class FT>
typename FaceMesh<VT,PT,FT>::FaceId FaceMesh<VT,PT,FT>::allocateFaces(const Size& n, const Face& face)
{
    FaceId newFId = MeshFaceHandler<Face>::allocateFaces(n, face);
    updateComponents();
    return newFId;
}

template<class VT, class PT, class FT>
Size FaceMesh<VT,PT,FT>::faceVertexNumber(const FaceId& id) const
{
    return MeshFaceHandler<Face>::face(id).vertexNumber();
}

template<class VT, class PT, class FT>
typename FaceMesh<VT,PT,FT>::FaceContainer& FaceMesh<VT,PT,FT>::faceVertexIds(const FaceId& id)
{
    return MeshFaceHandler<Face>::face(id).vertexIds();
}

template<class VT, class PT, class FT>
const typename FaceMesh<VT,PT,FT>::FaceContainer& FaceMesh<VT,PT,FT>::faceVertexIds(const FaceId& id) const
{
    return MeshFaceHandler<Face>::face(id).vertexIds();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::setFaceVertexIds(const FaceId& id, const FaceContainer& vertexIds)
{
    MeshFaceHandler<Face>::face(id).setVertexIds(vertexIds);
}

template<class VT, class PT, class FT>
template<class T>
void FaceMesh<VT,PT,FT>::setFaceVertexIds(const FaceId& id, const T& vertexIds)
{
    std::vector<VertexId> vector(vertexIds.begin(), vertexIds.end());
    MeshFaceHandler<Face>::face(id).setVertexIds(vector);
}

template<class VT, class PT, class FT>
typename FaceMesh<VT,PT,FT>::VertexId FaceMesh<VT,PT,FT>::faceVertexId(const FaceId& id, const Index& pos)
{
    return MeshFaceHandler<Face>::face(id).vertexId(pos);
}

template<class VT, class PT, class FT>
const typename FaceMesh<VT,PT,FT>::VertexId FaceMesh<VT,PT,FT>::faceVertexId(const FaceId& id, const Index& pos) const
{
    return MeshFaceHandler<Face>::face(id).vertexId(pos);
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::setFaceVertexId(const FaceId& id, const Index& pos, const VertexId& vId)
{
    MeshFaceHandler<Face>::face(id).setVertexId(pos, vId);
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::computeNormals()
{
    computeFaceNormals();
    computeVertexNormals();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::computeFaceNormals()
{
    meshComputeFaceNormalsSVDFitting(*this);
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::computeVertexNormals()
{
    meshComputeVertexNormalsFromFaceNormals(*this);
}

template<class VT, class PT, class FT>
bool FaceMesh<VT,PT,FT>::hasFaceNormals() const
{
    return vFaceNormalsEnabled;
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::enableFaceNormals()
{
    vFaceNormalsEnabled = true;
    updateFaceNormals();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::disableFaceNormals()
{
    vFaceNormalsEnabled = false;
    updateFaceNormals();
}

template<class VT, class PT, class FT>
bool FaceMesh<VT,PT,FT>::hasFaceMaterials() const
{
    return vFaceMaterialEnabled;
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::enableFaceMaterials()
{
    vFaceMaterialEnabled = true;
    updateFaceMaterials();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::disableFaceMaterials()
{
    vFaceMaterialEnabled = false;
    updateFaceMaterials();
}

template<class VT, class PT, class FT>
bool FaceMesh<VT,PT,FT>::hasWedgeNormals() const
{
    return vWedgeNormalsEnabled;
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::enableWedgeNormals()
{
    vWedgeNormalsEnabled = true;
    updateFaceWedgeNormals();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::disableWedgeNormals()
{
    vWedgeNormalsEnabled = false;
    updateFaceWedgeNormals();
}

template<class VT, class PT, class FT>
bool FaceMesh<VT,PT,FT>::hasWedgeUVs() const
{
    return vWedgeUVsEnabled;
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::enableWedgeUVs()
{
    vWedgeUVsEnabled = true;
    updateFaceWedgeUVs();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::disableWedgeUVs()
{
    vWedgeUVsEnabled = false;
    updateFaceWedgeUVs();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::compactAll()
{
    compactVertices();
    this->compactFaces();
    compactFaces();
    compactMaterials();
    compactWedgeNormals();
    compactWedgeUVs();
}

template<class VT, class PT, class FT>
std::vector<typename FaceMesh<VT,PT,FT>::MaterialId> FaceMesh<VT,PT,FT>::compactMaterials()
{
    std::vector<MaterialId> mMap = this->vMaterials.compact();

    //Update vertex ids in faces
    if (this->hasFaceMaterials()) {
        for (Face& face : this->faces()) {
            MaterialId oldId = this->faceMaterial(face);
            if (oldId != NULL_ID) {
                MaterialId newId = mMap.at(oldId);
                this->setFaceMaterial(face, newId);
            }
        }
    }

    return mMap;
}

template<class VT, class PT, class FT>
std::vector<typename FaceMesh<VT,PT,FT>::WedgeNormalId> FaceMesh<VT,PT,FT>::compactWedgeNormals()
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

template<class VT, class PT, class FT>
std::vector<typename FaceMesh<VT,PT,FT>::WedgeUVId> FaceMesh<VT,PT,FT>::compactWedgeUVs()
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

template<class VT, class PT, class FT>
std::vector<typename FaceMesh<VT,PT,FT>::FaceId> FaceMesh<VT,PT,FT>::compactFaces()
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

    //Compact face wedge normals
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

    //Compact face wedge UVs
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

template<class VT, class PT, class FT>
std::vector<typename FaceMesh<VT,PT,FT>::VertexId> FaceMesh<VT,PT,FT>::compactVertices()
{
    std::vector<VertexId> vMap = PolylineMesh<VT,PT>::compactVertices();

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

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::clearFaces()
{
    MeshFaceHandler<Face>::clearFaces();
    updateComponents();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::clear()
{
    PolylineMesh<VT,PT>::clear();
    this->clearFaces();
    this->clearMaterials();
    this->clearWedgeNormals();
    this->clearWedgeUVs();
    updateComponents();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::updateComponents()
{
    updateFaceMaterials();
    updateFaceNormals();
    updateFaceWedgeNormals();
    updateFaceWedgeUVs();
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::updateFaceMaterials()
{
    if (vFaceMaterialEnabled) {
        this->vFaceMaterials.resize(this->nextFaceId(), NULL_ID);
    }
    else {
        this->vFaceMaterials.clear();
    }
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::updateFaceNormals()
{
    if (vFaceNormalsEnabled) {
        this->vFaceNormals.resize(this->nextFaceId());
    }
    else {
        this->vFaceNormals.clear();
    }
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::updateFaceWedgeNormals()
{
    if (vWedgeNormalsEnabled) {
        this->vFaceWedgeNormals.resize(this->nextFaceId());
    }
    else {
        this->vFaceWedgeNormals.clear();
    }
}

template<class VT, class PT, class FT>
void FaceMesh<VT,PT,FT>::updateFaceWedgeUVs()
{
    if (vWedgeUVsEnabled) {
        this->vFaceWedgeUVs.resize(this->nextFaceId());
    }
    else {
        this->vFaceWedgeUVs.clear();
    }
}

template<class VT, class PT, class FT>
std::ostream& operator<<(std::ostream& output, const FaceMesh<VT,PT,FT>& faceMesh)
{
    const PolylineMesh<VT,PT>& eMesh = faceMesh;
    output << eMesh;

    output << "Faces: " << faceMesh.faceNumber() << std::endl;
    for (const typename FaceMesh<VT,PT,FT>::Face& face : faceMesh.faces()) {
        output << face << std::endl;
    }

    return output;
}

}
