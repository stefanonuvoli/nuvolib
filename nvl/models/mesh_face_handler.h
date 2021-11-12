#ifndef NVL_MODELS_MESH_FACE_HANDLER_H
#define NVL_MODELS_MESH_FACE_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class T>
class MeshFaceHandler
{

public:

    /* Typedefs */

    typedef T Face;

    typedef typename Face::FaceId FaceId;

    typedef typename Face::FaceNormal FaceNormal;

    typedef typename Face::Vertex::VertexNormal VertexNormal;
    typedef typename Face::Vertex::UV UV;

    typedef typename Face::Material Material;
    typedef typename Face::MaterialId MaterialId;

    typedef VectorWithDelete<Face> FaceContainer;


    /* Constructors */

    MeshFaceHandler();


    /* Faces */

    Size faceNumber() const;
    FaceId nextFaceId() const;

    Face& face(const FaceId& id);
    const Face& face(const FaceId& id) const;

    FaceId addFace(const Face& face);
    template<class... Ts>
    FaceId addFace(const Ts... vertices);

    void deleteFace(const FaceId& id);
    bool isFaceDeleted(const FaceId& id) const;

    void deleteFace(const Face& face);
    bool isFaceDeleted(const Face& face) const;

    IteratorWrapper<VectorWithDelete<Face>, typename VectorWithDelete<Face>::iterator> faces();
    IteratorWrapper<const VectorWithDelete<Face>, typename VectorWithDelete<Face>::const_iterator> faces() const;

    void clearFaces();


    /* Materials */

    Size materialNumber() const;
    MaterialId nextMaterialId() const;

    Material& material(const MaterialId& id);
    const Material& material(const MaterialId& id) const;

    MaterialId addMaterial(const Material& material);

    void deleteMaterial(const MaterialId& id);
    bool isMaterialDeleted(const MaterialId& id) const;

    void deleteMaterial(const Material& material);
    bool isMaterialDeleted(const Material& material) const;

    IteratorWrapper<VectorWithDelete<Material>, typename VectorWithDelete<Material>::iterator> materials();
    IteratorWrapper<const VectorWithDelete<Material>, typename VectorWithDelete<Material>::const_iterator> materials() const;

    void clearMaterials();

    bool hasFaceMaterial(const FaceId& id) const;
    Material& faceMaterial(const FaceId& id);
    const Material& faceMaterial(const FaceId& id) const;
    void setFaceMaterial(const FaceId& id, MaterialId materialId);
    void unsetFaceMaterial(const FaceId& id);

    bool hasFaceMaterial(const Face& face) const;
    Material& faceMaterial(const Face& face);
    const Material& faceMaterial(const Face& face) const;
    void setFaceMaterial(const Face& face, MaterialId faceMaterialId);
    void unsetFaceMaterial(const Face& face);


    /* Face normals */

    FaceNormal& faceNormal(const FaceId& id);
    const FaceNormal& faceNormal(const FaceId& id) const;
    void setFaceNormal(const FaceId& id, const FaceNormal& faceNormal);

    FaceNormal& faceNormal(const Face& face);
    const FaceNormal& faceNormal(const Face& face) const;
    void setFaceNormal(const Face& face, const FaceNormal& faceNormal);



    /* Face wedge normals */

    bool hasFaceWedgeVertexNormals(const FaceId& id) const;
    const std::vector<VertexNormal>& faceWedgeVertexNormals(const FaceId& id) const;
    void setFaceWedgeVertexNormals(const FaceId& id, const std::vector<VertexNormal>& vertexNormals);
    void clearFaceWedgeVertexNormals(const FaceId& id);

    bool hasFaceWedgeVertexNormals(const Face& face) const;
    const std::vector<VertexNormal>& faceWedgeVertexNormals(const Face& face) const;
    void setFaceWedgeVertexNormals(const Face& face, const std::vector<VertexNormal>& vertexNormals);
    void clearFaceWedgeVertexNormals(const Face& face);


    /* Face wedge UV coords */

    bool hasFaceWedgeUV(const FaceId& id) const;
    const std::vector<UV>& faceWedgeUV(const FaceId& id) const;
    void setFaceWedgeUV(const FaceId& id, const std::vector<UV>& uvCoords);
    void clearFaceWedgeUV(const FaceId& id);

    bool hasFaceWedgeUV(const Face& face) const;
    const std::vector<UV>& faceWedgeUV(const Face& face) const;
    void setFaceWedgeUV(const Face& face, const std::vector<UV>& uvCoords);
    void clearFaceWedgeUV(const Face& face);


protected:

    VectorWithDelete<Face> vFaces;

    VectorWithDelete<Material> vMaterials;

};

}

#include "mesh_face_handler.cpp"

#endif // NVL_MODELS_MESH_FACE_HANDLER_H
