#ifndef NVL_MODELS_MESH_WEDGE_NORMAL_HANDLER_H
#define NVL_MODELS_MESH_WEDGE_NORMAL_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class N>
class MeshWedgeNormalHandler
{

public:

    /* Typedefs */

    typedef N WedgeNormal;
    typedef Index WedgeNormalId;

    typedef VectorWithDelete<WedgeNormal> Container;


    /* Constructors */

    MeshWedgeNormalHandler();


    /* WedgeNormals */

    Size wedgeNormalNumber() const;
    WedgeNormalId nextWedgeNormalId() const;

    WedgeNormal& wedgeNormal(const WedgeNormalId& id);
    const WedgeNormal& wedgeNormal(const WedgeNormalId& id) const;

    WedgeNormalId addWedgeNormal(const WedgeNormal& wedgeNormal);

    WedgeNormalId allocateWedgeNormals(const Size& n);
    WedgeNormalId allocateWedgeNormals(const Size& n, const WedgeNormal& wedgeNormal);

    void deleteWedgeNormal(const WedgeNormalId& id);
    bool isWedgeNormalDeleted(const WedgeNormalId& id) const;

    IteratorWrapper<Container, typename Container::iterator> wedgeNormals();
    IteratorWrapper<const Container, typename Container::const_iterator> wedgeNormals() const;

    void clearWedgeNormals();



protected:

    Container vWedgeNormals;

};

}

#include "mesh_wedge_normal_handler.cpp"

#endif // NVL_MODELS_MESH_WEDGE_NORMAL_HANDLER_H
