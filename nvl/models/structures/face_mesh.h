#ifndef NVL_MODELS_FACE_MESH_H
#define NVL_MODELS_FACE_MESH_H

#include <nvl/nuvolib.h>

#include <nvl/models/structures/polyline_mesh.h>

#include <nvl/models/structures/handlers/mesh_face_handler.h>
#include <nvl/models/structures/handlers/mesh_material_handler.h>
#include <nvl/models/structures/handlers/mesh_face_material_handler.h>
#include <nvl/models/structures/handlers/mesh_face_normal_handler.h>
#include <nvl/models/structures/handlers/mesh_wedge_normal_handler.h>
#include <nvl/models/structures/handlers/mesh_wedge_uv_handler.h>
#include <nvl/models/structures/handlers/mesh_face_wedge_normal_handler.h>
#include <nvl/models/structures/handlers/mesh_face_wedge_uv_handler.h>

#include <ostream>

namespace nvl {

template<class F, class N, class M>
class FaceMeshTypes;

template<class VT, class PT, class FT>
class FaceMesh :
        public PolylineMesh<VT,PT>,
        public MeshFaceHandler<typename FT::Face>,
        public MeshMaterialHandler<typename FT::Material>,
        public MeshFaceMaterialHandler<typename FT::Face, typename FT::Material>,
        public MeshFaceNormalHandler<typename FT::Face, typename FT::Normal>,
        public MeshWedgeNormalHandler<typename VT::Normal>,
        public MeshFaceWedgeNormalHandler<typename FT::Face, typename VT::Normal>,
        public MeshWedgeUVHandler<typename VT::UV>,
        public MeshFaceWedgeUVHandler<typename FT::Face, typename VT::UV>
{

public:

    /* Typedefs  */

    typedef typename FT::Face Face;
    typedef typename FT::Normal FaceNormal;
    typedef typename FT::Material Material;

    typedef typename Material::Color MaterialColor;
    typedef typename Face::Container FaceContainer;

    /* Parent typedefs  */

    typedef typename PolylineMesh<VT,PT>::Polyline Polyline;
    typedef typename PolylineMesh<VT,PT>::PolylineColor PolylineColor;
    typedef typename PolylineMesh<VT,PT>::PolylineContainer PolylineContainer;

    typedef typename VertexMesh<VT>::Vertex Vertex;
    typedef typename VertexMesh<VT>::VertexId VertexId;
    typedef typename VertexMesh<VT>::Point Point;
    typedef typename VertexMesh<VT>::Scalar Scalar;
    typedef typename VertexMesh<VT>::VertexNormal VertexNormal;
    typedef typename VertexMesh<VT>::VertexColor VertexColor;
    typedef typename VertexMesh<VT>::VertexUV VertexUV;

    /* Handlers typedefs  */

    typedef typename MeshFaceHandler<Face>::FaceId FaceId;
    typedef typename MeshFaceMaterialHandler<Face, Material>::MaterialId MaterialId;
    typedef typename MeshFaceWedgeNormalHandler<Face,VertexNormal>::WedgeNormalId WedgeNormalId;
    typedef typename MeshFaceWedgeUVHandler<Face,VertexUV>::WedgeUVId WedgeUVId;


    /* Constructors */

    FaceMesh();


    /* Methods */    

    FaceId addFace(const Face& face);
    template<class... Ts>
    FaceId addFace(const Ts... vertices);

    FaceId allocateFaces(const Size& n);
    FaceId allocateFaces(const Size& n, const Face& face);

    Size faceVertexNumber(const FaceId& id) const;

    FaceContainer& faceVertexIds(const FaceId& id);
    const FaceContainer& faceVertexIds(const FaceId& id) const;
    void setFaceVertexIds(const FaceId& id, const FaceContainer& vertexIds);
    template<class T>
    void setFaceVertexIds(const FaceId& id, const T& vertexIds);

    VertexId faceVertexId(const FaceId& id, const Index& pos);
    const VertexId faceVertexId(const FaceId& id, const Index& pos) const;
    void setFaceVertexId(const FaceId& id, const Index& pos, const VertexId& vertexId);

    bool hasFaceNormals() const;
    void enableFaceNormals();
    void disableFaceNormals();
    bool hasFaceMaterials() const;
    void enableFaceMaterials();
    void disableFaceMaterials();
    bool hasWedgeNormals() const;
    void enableWedgeNormals();
    void disableWedgeNormals();
    bool hasWedgeUVs() const;
    void enableWedgeUVs();
    void disableWedgeUVs();

    void computeNormals();
    void computeFaceNormals();
    void computeVertexNormals();

    virtual void compactAll() override;
    std::vector<FaceId> compactFaces();
    std::vector<MaterialId> compactMaterials();
    std::vector<WedgeNormalId> compactWedgeNormals();
    std::vector<WedgeUVId> compactWedgeUVs();

    std::vector<VertexId> compactVertices() override;

    void clearFaces();
    void clear();

protected:

    bool vFaceNormalsEnabled;
    bool vFaceMaterialEnabled;
    bool vWedgeNormalsEnabled;
    bool vWedgeUVsEnabled;

    void updateComponents();
    void updateFaceMaterials();
    void updateFaceNormals();
    void updateFaceWedgeNormals();
    void updateFaceWedgeUVs();

};

template<class F, class N, class M>
class FaceMeshTypes
{

public:

    typedef F Face;
    typedef N Normal;
    typedef M Material;

};

template<class VT, class PT, class FT>
std::ostream& operator<<(std::ostream& output, const FaceMesh<VT,PT,FT>& mesh);

}

#include "face_mesh.cpp"

#endif // NVL_MODELS_FACE_MESH_H
