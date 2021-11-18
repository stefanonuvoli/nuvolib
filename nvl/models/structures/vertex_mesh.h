#ifndef NVL_MODELS_VERTEX_MESH_H
#define NVL_MODELS_VERTEX_MESH_H

#include <nvl/nuvolib.h>

#include <nvl/models/structures/abstract_mesh.h>

#include <nvl/models/handlers/mesh_vertex_handler.h>
#include <nvl/models/handlers/mesh_vertex_normal_handler.h>
#include <nvl/models/handlers/mesh_vertex_color_handler.h>
#include <nvl/models/handlers/mesh_vertex_uv_handler.h>

#include <ostream>

namespace nvl {

template<class V>
class VertexMesh :
        public AbstractMesh,
        public MeshVertexHandler<V>,
        public MeshVertexNormalHandler<V, typename V::Normal>,
        public MeshVertexColorHandler<V, typename V::Color>,
        public MeshVertexUVHandler<V, typename V::UV>
{

public:

    /* Typedefs */

    typedef typename MeshVertexHandler<V>::Vertex Vertex;
    typedef typename MeshVertexHandler<V>::VertexId VertexId;

    typedef typename Vertex::Color VertexColor;
    typedef typename Vertex::Normal VertexNormal;
    typedef typename Vertex::UV UV;
    typedef typename Vertex::Point Point;
    typedef typename Vertex::Scalar Scalar;

    typedef typename MeshVertexHandler<V>::Container Container;

    /* Constructors */

    VertexMesh();


    /* Methods */    

    VertexId addVertex(const Vertex& vertex);
    VertexId addVertex(const Point& point);
    template<class... Ts>
    VertexId addVertex(const Ts... points);

    VertexId allocateVertices(const Size& n);
    VertexId allocateVertices(const Size& n, const Vertex& vertex);    

    Point& vertexPoint(const VertexId& id);
    const Point& vertexPoint(const VertexId& id) const;
    void setVertexPoint(const VertexId& id, const Point& point);

    bool hasVertexNormals() const;
    void enableVertexNormals();
    void disableVertexNormals();
    bool hasVertexColors() const;
    void enableVertexColors();
    void disableVertexColors();
    bool hasVertexUVs() const;
    void enableVertexUVs();
    void disableVertexUVs();

    virtual void compactAll();
    virtual std::vector<VertexId> compactVertices();

    void clearVertices();
    void clear();


protected:

    bool vVertexNormalsEnabled;
    bool vVertexColorsEnabled;
    bool vVertexUVsEnabled;

    void updateComponents();
    void updateVertexNormals();
    void updateVertexColors();
    void updateVertexUVs();

};

template<class V>
std::ostream& operator<<(std::ostream& output, const VertexMesh<V>& vertexMesh);

}



#include "vertex_mesh.cpp"

#endif // NVL_MODELS_VERTEX_MESH_H
