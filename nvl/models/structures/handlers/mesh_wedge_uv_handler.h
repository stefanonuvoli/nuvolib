#ifndef NVL_MODELS_MESH_WEDGE_UV_HANDLER_H
#define NVL_MODELS_MESH_WEDGE_UV_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/containers/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class U>
class MeshWedgeUVHandler
{

public:

    /* Typedefs */

    typedef U VertexUV;
    typedef Index WedgeUVId;

    typedef VectorWithDelete<VertexUV> Container;


    /* Constructors */

    MeshWedgeUVHandler();


    /* WedgeUVs */

    Size wedgeUVNumber() const;
    WedgeUVId nextWedgeUVId() const;

    VertexUV& wedgeUV(const WedgeUVId& id);
    const VertexUV& wedgeUV(const WedgeUVId& id) const;

    WedgeUVId addWedgeUV(const VertexUV& wedgeUV);

    WedgeUVId allocateWedgeUVs(const Size& n);
    WedgeUVId allocateWedgeUVs(const Size& n, const VertexUV& wedgeUV);

    void deleteWedgeUV(const WedgeUVId& id);
    bool isWedgeUVDeleted(const WedgeUVId& id) const;

    IteratorWrapper<Container, typename Container::iterator> wedgeUVs();
    IteratorWrapper<const Container, typename Container::const_iterator> wedgeUVs() const;

    void clearWedgeUVs();



protected:

    Container vWedgeUVs;

};

}

#include "mesh_wedge_uv_handler.cpp"

#endif // NVL_MODELS_MESH_WEDGE_UV_HANDLER_H
