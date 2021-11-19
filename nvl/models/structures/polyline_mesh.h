#ifndef NVL_MODELS_POLYLINE_MESH_H
#define NVL_MODELS_POLYLINE_MESH_H

#include <nvl/nuvolib.h>

#include <nvl/models/structures/vertex_mesh.h>

#include <nvl/models/structures/handlers/mesh_polyline_handler.h>
#include <nvl/models/structures/handlers/mesh_polyline_color_handler.h>

#include <ostream>

namespace nvl {

template<class V, class C>
class PolylineMeshTypes;

template<class VT, class PT>
class PolylineMesh :
        public VertexMesh<VT>,
        public MeshPolylineHandler<typename PT::Polyline>,
        public MeshPolylineColorHandler<typename PT::Polyline, typename PT::Color>
{

public:

    /* Typedefs  */

    typedef typename PT::Polyline Polyline;
    typedef typename PT::Color PolylineColor;

    typedef typename Polyline::Container PolylineContainer;

    /* Parent typedefs  */

    typedef typename VertexMesh<VT>::Vertex Vertex;
    typedef typename VertexMesh<VT>::VertexId VertexId;
    typedef typename VertexMesh<VT>::Point Point;
    typedef typename VertexMesh<VT>::Scalar Scalar;
    typedef typename VertexMesh<VT>::VertexNormal VertexNormal;
    typedef typename VertexMesh<VT>::VertexColor VertexColor;
    typedef typename VertexMesh<VT>::VertexUV VertexUV;

    /* Handlers typedefs  */

    typedef typename MeshPolylineHandler<Polyline>::PolylineId PolylineId;


    /* Constructors */

    PolylineMesh();


    /* Methods */

    PolylineId addPolyline(const Polyline& polyline);
    template<class... Ts>
    PolylineId addPolyline(const Ts... vertices);

    PolylineId allocatePolylines(const Size& n);
    PolylineId allocatePolylines(const Size& n, const Polyline& polyline);

    Size polylineVertexNumber(const PolylineId& id) const;

    PolylineContainer& polylineVertexIds(const PolylineId& id);
    const PolylineContainer& polylineVertexIds(const PolylineId& id) const;
    void setPolylineVertexIds(const PolylineId& id, const PolylineContainer& vertexIds);

    VertexId polylineVertexId(const PolylineId& id, const Index& pos);
    const VertexId polylineVertexId(const PolylineId& id, const Index& pos) const;
    void setPolylineVertexId(const PolylineId& id, const Index& pos, const VertexId& vId);

    bool hasPolylineColors() const;
    void enablePolylineColors();
    void disablePolylineColors();

    void compactAll();
    std::vector<PolylineId> compactPolylines();

    std::vector<VertexId> compactVertices() override;

    void clearPolylines();
    void clear();

protected:

    bool vPolylineColorsEnabled;

    void updateComponents();
    void updatePolylineColors();

};

template<class P, class C>
class PolylineMeshTypes
{

public:

    typedef P Polyline;
    typedef C Color;

};


template<class VT, class PT>
std::ostream& operator<<(std::ostream& output, const PolylineMesh<VT,PT>& mesh);

}

#include "polyline_mesh.cpp"

#endif // NVL_MODELS_POLYLINE_MESH_H
