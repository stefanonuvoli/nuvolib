/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_VERTEX_MESH_H
#define NVL_MODELS_VERTEX_MESH_H

#include <nvl/nuvolib.h>

#include <nvl/models/structures/abstract_mesh.h>

#include <nvl/models/structures/handlers/mesh_vertex_handler.h>
#include <nvl/models/structures/handlers/mesh_vertex_normal_handler.h>
#include <nvl/models/structures/handlers/mesh_vertex_color_handler.h>
#include <nvl/models/structures/handlers/mesh_vertex_uv_handler.h>

#include <ostream>

namespace nvl {

template<class V, class N, class C, class U>
class VertexMeshTypes;

template<class VT>
class VertexMesh :
        public AbstractMesh,
        public MeshVertexHandler<typename VT::Vertex>,
        public MeshVertexNormalHandler<typename VT::Vertex, typename VT::Normal>,
        public MeshVertexColorHandler<typename VT::Vertex, typename VT::Color>,
        public MeshVertexUVHandler<typename VT::Vertex, typename VT::UV>
{

public:

    /* Typedefs */

    typedef typename VT::Vertex Vertex;

    typedef typename VT::Normal VertexNormal;
    typedef typename VT::Color VertexColor;
    typedef typename VT::UV VertexUV;

    typedef typename Vertex::Point Point;
    typedef typename Vertex::Scalar Scalar;

    /* Handlers typedefs  */

    typedef typename MeshVertexHandler<Vertex>::VertexId VertexId;


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

template<class V, class N, class C, class U>
class VertexMeshTypes
{

public:

    typedef V Vertex;
    typedef N Normal;
    typedef C Color;
    typedef U UV;

};

template<class VT>
std::ostream& operator<<(std::ostream& output, const VertexMesh<VT>& vertexMesh);

}



#include "vertex_mesh.cpp"

#endif // NVL_MODELS_VERTEX_MESH_H
