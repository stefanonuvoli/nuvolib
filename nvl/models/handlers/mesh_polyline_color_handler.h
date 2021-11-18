#ifndef NVL_MODELS_MESH_POLYLINE_COLOR_HANDLER_H
#define NVL_MODELS_MESH_POLYLINE_COLOR_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class P, class C>
class MeshPolylineColorHandler
{

public:

    /* Typedefs */

    typedef P Polyline;
    typedef C Color;

    typedef typename P::Id PolylineId;

    typedef VectorWithDelete<Color> Container;


    /* Constructors */

    MeshPolylineColorHandler();


    /* Methods */

    Color& polylineColor(const PolylineId& id);
    const Color& polylineColor(const PolylineId& id) const;
    void setPolylineColor(const PolylineId& id, const Color& color);

    Color& polylineColor(const Polyline& polyline);
    const Color& polylineColor(const Polyline& polyline) const;
    void setPolylineColor(const Polyline& polyline, const Color& color);


protected:

    Container vPolylineColors;

};

}

#include "mesh_polyline_color_handler.cpp"

#endif // NVL_MODELS_MESH_POLYLINE_COLOR_HANDLER_H
