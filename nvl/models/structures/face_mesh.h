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

template<class V, class L, class F>
class FaceMesh :
        public PolylineMesh<V,L>,
        public MeshFaceHandler<F>,
        public MeshMaterialHandler<typename F::Material>,
        public MeshFaceMaterialHandler<F, typename F::Material>,
        public MeshFaceNormalHandler<F, typename F::Normal>,
        public MeshWedgeNormalHandler<typename F::Normal>,
        public MeshFaceWedgeNormalHandler<F, typename F::Normal>,
        public MeshWedgeUVHandler<typename F::UV>,
        public MeshFaceWedgeUVHandler<F, typename F::UV>
{

public:

    /* Typedefs  */

    typedef typename MeshFaceHandler<F>::Face Face;
    typedef typename MeshFaceHandler<F>::FaceId FaceId;

    typedef typename Face::Normal FaceNormal;
    typedef typename Face::Material Material;
    typedef typename Material::Id MaterialId;
    typedef typename Material::Color MaterialColor;

    typedef typename Face::VertexNormal WedgeNormal;
    typedef typename Face::UV WedgeUV;

    typedef typename MeshFaceWedgeNormalHandler<F,WedgeNormal>::WedgeNormalId WedgeNormalId;
    typedef typename MeshFaceWedgeUVHandler<F,WedgeUV>::WedgeUVId WedgeUVId;

    typedef typename PolylineMesh<V,L>::Vertex Vertex;
    typedef typename PolylineMesh<V,L>::VertexId VertexId;
    typedef typename PolylineMesh<V,L>::VertexColor VertexColor;
    typedef typename PolylineMesh<V,L>::VertexNormal VertexNormal;
    typedef typename PolylineMesh<V,L>::UV UV;
    typedef typename PolylineMesh<V,L>::Scalar Scalar;
    typedef typename PolylineMesh<V,L>::Point Point;

    typedef typename PolylineMesh<V,L>::PolylineId PolylineId;
    typedef typename PolylineMesh<V,L>::Polyline Polyline;
    typedef typename PolylineMesh<V,L>::PolylineColor PolylineColor;


    /* Constructors */

    FaceMesh();


    /* Methods */    

    FaceId addFace(const Face& face);
    template<class... Ts>
    FaceId addFace(const Ts... vertices);

    FaceId allocateFaces(const Size& n);
    FaceId allocateFaces(const Size& n, const Face& face);

    Size faceVertexNumber(const FaceId& id) const;

    typename F::VertexContainer& faceVertexIds(const FaceId& id);
    const typename F::VertexContainer& faceVertexIds(const FaceId& id) const;
    void setFaceVertexIds(const FaceId& id, const typename F::VertexContainer& vertexIds);

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

template<class V, class L, class F>
std::ostream& operator<<(std::ostream& output, const FaceMesh<V,L,F>& mesh);

}

#include "face_mesh.cpp"

#endif // NVL_MODELS_FACE_MESH_H
