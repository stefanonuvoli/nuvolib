/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_WEDGE_NORMAL_HANDLER_H
#define NVL_MODELS_MESH_WEDGE_NORMAL_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/containers/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class N>
class MeshWedgeNormalHandler
{

public:

    /* Typedefs */

    typedef N VertexNormal;
    typedef Index WedgeNormalId;

    typedef VectorWithDelete<VertexNormal> Container;


    /* Constructors */

    MeshWedgeNormalHandler();


    /* WedgeNormals */

    Size wedgeNormalNumber() const;
    WedgeNormalId nextWedgeNormalId() const;

    VertexNormal& wedgeNormal(const WedgeNormalId& id);
    const VertexNormal& wedgeNormal(const WedgeNormalId& id) const;

    WedgeNormalId addWedgeNormal(const VertexNormal& wedgeNormal);

    WedgeNormalId allocateWedgeNormals(const Size& n);
    WedgeNormalId allocateWedgeNormals(const Size& n, const VertexNormal& wedgeNormal);

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
