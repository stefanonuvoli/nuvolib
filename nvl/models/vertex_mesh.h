#ifndef NVL_MODELS_VERTEX_MESH_H
#define NVL_MODELS_VERTEX_MESH_H

#include <nvl/nuvolib.h>

#include <nvl/models/abstract_mesh.h>

#include <nvl/models/mesh_vertex_handler.h>

#include <ostream>

namespace nvl {

template<class V>
class VertexMesh :
        public AbstractMesh,
        public MeshVertexHandler<V>
{

public:

    /* Typedefs */

    typedef typename MeshVertexHandler<V>::Vertex Vertex;
    typedef typename MeshVertexHandler<V>::VertexId VertexId;

    typedef typename Vertex::VertexColor VertexColor;
    typedef typename Vertex::VertexNormal VertexNormal;
    typedef typename Vertex::UV UV;
    typedef typename Vertex::Point Point;
    typedef typename Vertex::Scalar Scalar;

    typedef typename MeshVertexHandler<V>::VertexContainer VertexContainer;

    /* Constructors */

    VertexMesh();


    /* Methods */

    void compactAll();
    std::vector<VertexId> compactVertices();

    void clear();

};

template<class V>
std::ostream& operator<<(std::ostream& output, const VertexMesh<V>& vertexMesh);

}



#include "vertex_mesh.cpp"

#endif // NVL_MODELS_VERTEX_MESH_H
