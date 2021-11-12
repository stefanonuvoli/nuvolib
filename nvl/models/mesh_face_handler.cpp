#include "mesh_face_handler.h"

namespace nvl {

template<class T>
MeshFaceHandler<T>::MeshFaceHandler()
{

}

template<class T>
Size MeshFaceHandler<T>::faceNumber() const
{
    return vFaces.size();
}

template<class T>
typename MeshFaceHandler<T>::FaceId MeshFaceHandler<T>::nextFaceId() const
{
    return vFaces.realSize();
}

template<class T>
typename MeshFaceHandler<T>::Face& MeshFaceHandler<T>::face(const FaceId& id)
{
    return vFaces[id];
}

template<class T>
const typename MeshFaceHandler<T>::Face& MeshFaceHandler<T>::face(const FaceId& id) const
{
    return vFaces[id];
}

template<class T>
typename MeshFaceHandler<T>::FaceId MeshFaceHandler<T>::addFace(const Face& face)
{
    assert(face.vertexNumber() >= 3);
    FaceId newId = nextFaceId();
    vFaces.push_back(face);
    vFaces[newId].setId(newId);
    return newId;
}

template<class T>
template<class... Ts>
typename MeshFaceHandler<T>::FaceId MeshFaceHandler<T>::addFace(const Ts... vertices)
{
    FaceId newId = nextFaceId();
    Face face(vertices...);
    assert(face.vertexNumber() >= 3);
    face.setId(newId);
    vFaces.push_back(face);
    return newId;
}

template<class T>
void MeshFaceHandler<T>::deleteFace(const FaceId& id)
{
    Index copyId = id;
    vFaces[copyId].setId(MAX_INDEX);
    vFaces.erase(copyId);
}

template<class T>
bool MeshFaceHandler<T>::isFaceDeleted(const FaceId& id) const
{
    return vFaces.isDeleted(id);
}

template<class T>
void MeshFaceHandler<T>::deleteFace(const Face& face)
{
    deleteFace(face.id());
}

template<class T>
bool MeshFaceHandler<T>::isFaceDeleted(const Face& face) const
{
    return isFaceDeleted(face.id());
}

template<class T>
IteratorWrapper<VectorWithDelete<typename MeshFaceHandler<T>::Face>, typename VectorWithDelete<typename MeshFaceHandler<T>::Face>::iterator> MeshFaceHandler<T>::faces()
{
    return IteratorWrapper<VectorWithDelete<Face>, typename VectorWithDelete<Face>::iterator>(&vFaces);
}

template<class T>
IteratorWrapper<const VectorWithDelete<typename MeshFaceHandler<T>::Face>, typename VectorWithDelete<typename MeshFaceHandler<T>::Face>::const_iterator> MeshFaceHandler<T>::faces() const
{
    return IteratorWrapper<const VectorWithDelete<Face>, typename VectorWithDelete<Face>::const_iterator>(&vFaces);
}

template<class T>
void MeshFaceHandler<T>::clearFaces()
{
    vFaces.clear();
}

template<class T>
Size MeshFaceHandler<T>::materialNumber() const
{
    return vMaterials.size();
}

template<class T>
typename MeshFaceHandler<T>::MaterialId MeshFaceHandler<T>::nextMaterialId() const
{
    return vMaterials.realSize();
}

template<class T>
typename MeshFaceHandler<T>::Material& MeshFaceHandler<T>::material(const MaterialId& id)
{
    return vMaterials[id];
}

template<class T>
const typename MeshFaceHandler<T>::Material& MeshFaceHandler<T>::material(const MaterialId& id) const
{
    return vMaterials[id];
}

template<class T>
typename MeshFaceHandler<T>::MaterialId MeshFaceHandler<T>::addMaterial(const Material& material)
{
    MaterialId newId = nextMaterialId();

    vMaterials.push_back(material);

    vMaterials[newId].setId(newId);

    return newId;
}

template<class T>
void MeshFaceHandler<T>::deleteMaterial(const MaterialId& id)
{
    Size currentId = id;
    vMaterials[id].setId(MAX_INDEX);
    vMaterials.erase(currentId);
}

template<class T>
bool MeshFaceHandler<T>::isMaterialDeleted(const MaterialId& id) const
{
    return vMaterials.isDeleted(id);
}

template<class T>
void MeshFaceHandler<T>::deleteMaterial(const Material& material)
{
    deleteMaterial(material.id());
}

template<class T>
bool MeshFaceHandler<T>::isMaterialDeleted(const Material& material) const
{
    return isMaterialDeleted(material.id());
}

template<class T>
IteratorWrapper<VectorWithDelete<typename MeshFaceHandler<T>::Material>, typename VectorWithDelete<typename MeshFaceHandler<T>::Material>::iterator> MeshFaceHandler<T>::materials()
{
    return IteratorWrapper<VectorWithDelete<Material>, typename VectorWithDelete<Material>::iterator>(&vMaterials);
}

template<class T>
IteratorWrapper<const VectorWithDelete<typename MeshFaceHandler<T>::Material>, typename VectorWithDelete<typename MeshFaceHandler<T>::Material>::const_iterator> MeshFaceHandler<T>::materials() const
{
    return IteratorWrapper<const VectorWithDelete<Material>, typename VectorWithDelete<Material>::const_iterator>(&vMaterials);
}

template<class T>
void MeshFaceHandler<T>::clearMaterials()
{
    vMaterials.clear();
}

template<class T>
bool MeshFaceHandler<T>::hasFaceMaterial(const FaceId& id) const
{
    return face(id).materialId() < MAX_INDEX && !vMaterials.isDeleted(face(id).materialId());
}

template<class T>
typename MeshFaceHandler<T>::Material& MeshFaceHandler<T>::faceMaterial(const FaceId& id)
{
    assert(face(id).materialId() < MAX_INDEX && !vMaterials.isDeleted(face(id).materialId()) && "Face has no material assigned.");

    return vMaterials[face(id).materialId()];
}

template<class T>
const typename MeshFaceHandler<T>::Material& MeshFaceHandler<T>::faceMaterial(const FaceId& id) const
{
    assert(face(id).materialId() < MAX_INDEX && !vMaterials.isDeleted(face(id).materialId()) && "Face has no material assigned.");

    return vMaterials[face(id).materialId()];
}

template<class T>
void MeshFaceHandler<T>::setFaceMaterial(const FaceId& id, MaterialId materialId)
{
    face(id).setMaterialId(materialId);
}

template<class T>
void MeshFaceHandler<T>::unsetFaceMaterial(const FaceId& id)
{
    assert(face(id).materialId() < MAX_INDEX && !vMaterials.isDeleted(face(id).materialId()) && "Face has no material assigned.");

    face(id).setMaterialId(MAX_INDEX);
}

template<class T>
bool MeshFaceHandler<T>::hasFaceMaterial(const Face& face) const
{
    return hasFaceMaterial(face.id());
}

template<class T>
typename MeshFaceHandler<T>::Material& MeshFaceHandler<T>::faceMaterial(const Face& face)
{
    return faceMaterial(face.id());
}

template<class T>
const typename MeshFaceHandler<T>::Material& MeshFaceHandler<T>::faceMaterial(const Face& face) const
{
    return faceMaterial(face.id());
}

template<class T>
void MeshFaceHandler<T>::setFaceMaterial(const Face& face, MaterialId materialId)
{
    setFaceMaterial(face.id(), materialId);
}

template<class T>
void MeshFaceHandler<T>::unsetFaceMaterial(const Face& face)
{
    unsetFaceMaterial(face.id());
}

template<class T>
typename MeshFaceHandler<T>::FaceNormal& MeshFaceHandler<T>::faceNormal(const FaceId& id)
{
    return face(id).normal();
}

template<class T>
const typename MeshFaceHandler<T>::FaceNormal& MeshFaceHandler<T>::faceNormal(const FaceId& id) const
{
    return face(id).normal();
}

template<class T>
void MeshFaceHandler<T>::setFaceNormal(const FaceId& id, const FaceNormal& faceNormal)
{
    face(id).setNormal(faceNormal);
}

template<class T>
typename MeshFaceHandler<T>::FaceNormal& MeshFaceHandler<T>::faceNormal(const Face& face)
{
    return faceNormal(face.id());
}

template<class T>
const typename MeshFaceHandler<T>::FaceNormal& MeshFaceHandler<T>::faceNormal(const Face& face) const
{
    return faceNormal(face.id());
}

template<class T>
void MeshFaceHandler<T>::setFaceNormal(const Face& face, const FaceNormal& faceNormal)
{
    setFaceNormal(face.id(), faceNormal);
}

template<class T>
bool MeshFaceHandler<T>::hasFaceWedgeVertexNormals(const FaceId& id) const
{
    return !face(id).wedgeVertexNormals().empty();
}

template<class T>
const std::vector<typename MeshFaceHandler<T>::VertexNormal>& MeshFaceHandler<T>::faceWedgeVertexNormals(const FaceId& id) const
{
    return face(id).wedgeVertexNormals();
}

template<class T>
void MeshFaceHandler<T>::setFaceWedgeVertexNormals(const FaceId& id, const std::vector<VertexNormal>& vertexNormals)
{
    face(id).setWedgeVertexNormals(vertexNormals);
}

template<class T>
void MeshFaceHandler<T>::clearFaceWedgeVertexNormals(const FaceId& id)
{
    face(id).clearWedgeVertexNormals();
}

template<class T>
bool MeshFaceHandler<T>::hasFaceWedgeVertexNormals(const Face& face) const
{
    return hasFaceWedgeVertexNormals(face.id());
}

template<class T>
const typename std::vector<typename MeshFaceHandler<T>::VertexNormal>& MeshFaceHandler<T>::faceWedgeVertexNormals(const Face& face) const
{
    return faceWedgeVertexNormals(face.id());
}

template<class T>
void MeshFaceHandler<T>::setFaceWedgeVertexNormals(const Face& face, const std::vector<VertexNormal>& vertexNormal)
{
    setFaceWedgeVertexNormals(face.id(), vertexNormal);
}

template<class T>
void MeshFaceHandler<T>::clearFaceWedgeVertexNormals(const Face& face)
{
    clearFaceWedgeVertexNormals(face.id());
}

template<class T>
bool MeshFaceHandler<T>::hasFaceWedgeUV(const FaceId& id) const
{
    return !face(id).wedgeUV().empty();
}

template<class T>
const std::vector<typename MeshFaceHandler<T>::UV>& MeshFaceHandler<T>::faceWedgeUV(const FaceId& id) const
{
    return face(id).wedgeUV();
}

template<class T>
void MeshFaceHandler<T>::setFaceWedgeUV(const FaceId& id, const std::vector<UV>& uvCoords)
{
    face(id).setWedgeUV(uvCoords);
}

template<class T>
void MeshFaceHandler<T>::clearFaceWedgeUV(const FaceId& id)
{
    face(id).clearWedgeUV();
}

template<class T>
bool MeshFaceHandler<T>::hasFaceWedgeUV(const Face& face) const
{
    return hasFaceWedgeUV(face.id());
}

template<class T>
const typename std::vector<typename MeshFaceHandler<T>::UV>& MeshFaceHandler<T>::faceWedgeUV(const Face& face) const
{
    return faceWedgeUV(face.id());
}

template<class T>
void MeshFaceHandler<T>::setFaceWedgeUV(const Face& face, const std::vector<UV>& uvCoords)
{
    setFaceWedgeUV(face.id(), uvCoords);
}

template<class T>
void MeshFaceHandler<T>::clearFaceWedgeUV(const Face& face)
{
    clearFaceWedgeUV(face.id());
}

}
