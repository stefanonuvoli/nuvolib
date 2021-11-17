#ifndef NVL_MODELS_MESH_POLYLINE_HANDLER_H
#define NVL_MODELS_MESH_POLYLINE_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class T>
class MeshPolylineHandler
{

public:

    /* Typedefs */

    typedef T Polyline;
    typedef typename Polyline::Id PolylineId;

    typedef typename Polyline::VertexId VertexId;

    typedef VectorWithDelete<Polyline> Container;

    /* Constructors */

    MeshPolylineHandler();


    /* Methods */

    Size polylineNumber() const;
    PolylineId nextPolylineId() const;

    Polyline& polyline(const PolylineId& id);
    const Polyline& polyline(const PolylineId& id) const;

    PolylineId addPolyline(const Polyline& polyline);
    template<class... Ts>
    PolylineId addPolyline(const Ts... vertices);

    PolylineId allocatePolylines(const Size& n);
    PolylineId allocatePolylines(const Size& n, const Polyline& polyline);

    void deletePolyline(const PolylineId& id);
    bool isPolylineDeleted(const PolylineId& id) const;

    void deletePolyline(const Polyline& polyline);
    bool isPolylineDeleted(const Polyline& polyline) const;

    IteratorWrapper<VectorWithDelete<Polyline>, typename VectorWithDelete<Polyline>::iterator> polylines();
    IteratorWrapper<const VectorWithDelete<Polyline>, typename VectorWithDelete<Polyline>::const_iterator> polylines() const;

    void clearPolylines();


protected:

    VectorWithDelete<Polyline> vPolylines;

};

}

#include "mesh_polyline_handler.cpp"

#endif // NVL_MODELS_MESH_POLYLINE_HANDLER_H
