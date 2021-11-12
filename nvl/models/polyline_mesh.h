#ifndef NVL_MODELS_POLYLINE_MESH_H
#define NVL_MODELS_POLYLINE_MESH_H

#include <nvl/nuvolib.h>

#include <nvl/models/vertex_mesh.h>

#include <nvl/models/mesh_polyline_handler.h>

#include <ostream>

namespace nvl {

template<class V, class L>
class PolylineMesh :
        public VertexMesh<V>,
        public MeshPolylineHandler<L>
{

public:

    /* Typedefs  */

    typedef typename VertexMesh<V>::VertexId VertexId;
    typedef typename VertexMesh<V>::Vertex Vertex;
    typedef typename VertexMesh<V>::VertexColor VertexColor;
    typedef typename VertexMesh<V>::VertexNormal VertexNormal;
    typedef typename VertexMesh<V>::UV UV;
    typedef typename VertexMesh<V>::Scalar Scalar;
    typedef typename VertexMesh<V>::Point Point;

    typedef typename MeshPolylineHandler<L>::Polyline Polyline;
    typedef typename MeshPolylineHandler<L>::PolylineId PolylineId;
    typedef typename Polyline::PolylineColor PolylineColor;

    typedef typename VertexMesh<V>::VertexContainer VertexContainer;
    typedef typename MeshPolylineHandler<L>::PolylineContainer PolylineContainer;


    /* Constructors */

    PolylineMesh();


    /* Methods */

    void compactAll();
    std::vector<VertexId> compactVertices();
    std::vector<PolylineId> compactPolylines();

    void clear();

};

template<class V, class L>
std::ostream& operator<<(std::ostream& output, const PolylineMesh<V,L>& mesh);

}

#include "polyline_mesh.cpp"

#endif // NVL_MODELS_POLYLINE_MESH_H
