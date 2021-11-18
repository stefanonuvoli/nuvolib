#ifndef NVL_MODELS_POLYLINE_MESH_H
#define NVL_MODELS_POLYLINE_MESH_H

#include <nvl/nuvolib.h>

#include <nvl/models/structures/vertex_mesh.h>

#include <nvl/models/handlers/mesh_polyline_handler.h>
#include <nvl/models/handlers/mesh_polyline_color_handler.h>

#include <ostream>

namespace nvl {

template<class V, class L>
class PolylineMesh :
        public VertexMesh<V>,
        public MeshPolylineHandler<L>,
        public MeshPolylineColorHandler<L, typename L::Color>
{

public:

    /* Typedefs  */

    typedef typename MeshPolylineHandler<L>::Polyline Polyline;
    typedef typename MeshPolylineHandler<L>::PolylineId PolylineId;

    typedef typename Polyline::Color PolylineColor;

    typedef typename VertexMesh<V>::VertexId VertexId;
    typedef typename VertexMesh<V>::Vertex Vertex;
    typedef typename VertexMesh<V>::VertexColor VertexColor;
    typedef typename VertexMesh<V>::VertexNormal VertexNormal;
    typedef typename VertexMesh<V>::UV UV;
    typedef typename VertexMesh<V>::Scalar Scalar;
    typedef typename VertexMesh<V>::Point Point;


    /* Constructors */

    PolylineMesh();


    /* Methods */

    PolylineId addPolyline(const Polyline& polyline);
    template<class... Ts>
    PolylineId addPolyline(const Ts... vertices);

    PolylineId allocatePolylines(const Size& n);
    PolylineId allocatePolylines(const Size& n, const Polyline& polyline);

    typename L::VertexContainer& polylineVertexIds(const PolylineId& id);
    const typename L::VertexContainer& polylineVertexIds(const PolylineId& id) const;
    void setPolylineVertexIds(const PolylineId& id, const typename L::VertexContainer& vertexIds);

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

template<class V, class L>
std::ostream& operator<<(std::ostream& output, const PolylineMesh<V,L>& mesh);

}

#include "polyline_mesh.cpp"

#endif // NVL_MODELS_POLYLINE_MESH_H
