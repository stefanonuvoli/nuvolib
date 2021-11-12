#ifndef NVL_MODELS_FACE_MESH_H
#define NVL_MODELS_FACE_MESH_H

#include <nvl/nuvolib.h>

#include <nvl/models/polyline_mesh.h>

#include <nvl/models/mesh_face_handler.h>

#include <ostream>

namespace nvl {

template<class V, class L, class F>
class FaceMesh :
        public PolylineMesh<V,L>,
        public MeshFaceHandler<F>
{

public:

    /* Typedefs  */

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

    typedef typename MeshFaceHandler<F>::FaceId FaceId;
    typedef typename MeshFaceHandler<F>::Face Face;
    typedef typename Face::FaceNormal FaceNormal;
    typedef typename Face::Material Material;
    typedef typename Face::MaterialId MaterialId;
    typedef typename Face::MaterialColor MaterialColor;

    typedef typename PolylineMesh<V,L>::VertexContainer VertexContainer;
    typedef typename PolylineMesh<V,L>::PolylineContainer PolylineContainer;
    typedef typename MeshFaceHandler<F>::FaceContainer FaceContainer;



    /* Constructors */

    FaceMesh();


    /* Methods */

    void computeNormals();
    void computeFaceNormals();
    void computeVertexNormals();

    void compactAll();
    std::vector<VertexId> compactVertices();
    std::vector<FaceId> compactFaces();
    std::vector<MaterialId> compactMaterials();

    void clear();

};

template<class V, class L, class F>
std::ostream& operator<<(std::ostream& output, const FaceMesh<V,L,F>& mesh);

}

#include "face_mesh.cpp"

#endif // NVL_MODELS_FACE_MESH_H
