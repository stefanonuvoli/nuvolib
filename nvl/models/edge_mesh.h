#ifndef NVL_MODELS_EDGE_MESH_H
#define NVL_MODELS_EDGE_MESH_H

#include <nvl/nuvolib.h>

#include <nvl/models/face_mesh.h>

#include <nvl/models/mesh_edge_handler.h>

#include <ostream>

namespace nvl {

template<class V, class L, class F, class E>
class EdgeMesh :
        public FaceMesh<V,L,F>,
        public MeshEdgeHandler<E>
{

public:

    /* Typedefs  */

    typedef typename FaceMesh<V,L,F>::Vertex Vertex;
    typedef typename FaceMesh<V,L,F>::VertexId VertexId;
    typedef typename FaceMesh<V,L,F>::VertexColor VertexColor;
    typedef typename FaceMesh<V,L,F>::VertexNormal VertexNormal;
    typedef typename FaceMesh<V,L,F>::UV UV;
    typedef typename FaceMesh<V,L,F>::Scalar Scalar;
    typedef typename FaceMesh<V,L,F>::Point Point;

    typedef typename FaceMesh<V,L,F>::PolylineId PolylineId;
    typedef typename FaceMesh<V,L,F>::Polyline Polyline;
    typedef typename FaceMesh<V,L,F>::PolylineColor PolylineColor;

    typedef typename FaceMesh<V,L,F>::FaceId FaceId;
    typedef typename FaceMesh<V,L,F>::Face Face;
    typedef typename FaceMesh<V,L,F>::FaceNormal FaceNormal;
    typedef typename FaceMesh<V,L,F>::Material Material;
    typedef typename FaceMesh<V,L,F>::MaterialId MaterialId;
    typedef typename Face::MaterialColor MaterialColor;

    typedef typename MeshEdgeHandler<E>::EdgeId EdgeId;
    typedef typename MeshEdgeHandler<E>::Edge Edge;

    typedef typename FaceMesh<V,L,F>::VertexContainer VertexContainer;
    typedef typename FaceMesh<V,L,F>::PolylineContainer PolylineContainer;
    typedef typename FaceMesh<V,L,F>::FaceContainer FaceContainer;
    typedef typename MeshEdgeHandler<E>::EdgeContainer EdgeContainer;


    /* Constructors */

    EdgeMesh();


    /* Methods */

    void compactAll();
    std::vector<VertexId> compactVertices();
    std::vector<FaceId> compactFaces();
    std::vector<EdgeId> compactEdges();

    void clear();

};

template<class V, class E, class F>
std::ostream& operator<<(std::ostream& output, const FaceMesh<V,E,F>& mesh);

}

#include "edge_mesh.cpp"

#endif // NVL_MODELS_EDGE_MESH_H
