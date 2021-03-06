/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_wedge_uv_handler.h"

namespace nvl {

template<class U>
MeshWedgeUVHandler<U>::MeshWedgeUVHandler()
{

}

template<class U>
Size MeshWedgeUVHandler<U>::wedgeUVNumber() const
{
    return vWedgeUVs.size();
}

template<class U>
typename MeshWedgeUVHandler<U>::WedgeUVId MeshWedgeUVHandler<U>::nextWedgeUVId() const
{
    return vWedgeUVs.realSize();
}

template<class U>
typename MeshWedgeUVHandler<U>::VertexUV& MeshWedgeUVHandler<U>::wedgeUV(const WedgeUVId& id)
{
    return vWedgeUVs[id];
}

template<class U>
const typename MeshWedgeUVHandler<U>::VertexUV& MeshWedgeUVHandler<U>::wedgeUV(const WedgeUVId& id) const
{
    return vWedgeUVs[id];
}

template<class U>
typename MeshWedgeUVHandler<U>::WedgeUVId MeshWedgeUVHandler<U>::addWedgeUV(const VertexUV& wedgeUV)
{
    WedgeUVId newId = nextWedgeUVId();
    vWedgeUVs.push_back(wedgeUV);
    return newId;
}

template<class U>
typename MeshWedgeUVHandler<U>::WedgeUVId MeshWedgeUVHandler<U>::allocateWedgeUVs(const Size& n)
{
    return allocateWedgeUVs(n, VertexUV());
}

template<class U>
typename MeshWedgeUVHandler<U>::WedgeUVId MeshWedgeUVHandler<U>::allocateWedgeUVs(const Size& n, const VertexUV& wedgeUV)
{
    Index firstIndex = vWedgeUVs.realSize();
    Index lastIndex = firstIndex + n;

    vWedgeUVs.resize(lastIndex, wedgeUV);

    return firstIndex;
}

template<class U>
void MeshWedgeUVHandler<U>::deleteWedgeUV(const WedgeUVId& id)
{
    Size currentId = id;
    vWedgeUVs.erase(currentId);
}

template<class U>
bool MeshWedgeUVHandler<U>::isWedgeUVDeleted(const WedgeUVId& id) const
{
    return vWedgeUVs.isDeleted(id);
}

template<class U>
IteratorWrapper<typename MeshWedgeUVHandler<U>::Container, typename MeshWedgeUVHandler<U>::Container::iterator> MeshWedgeUVHandler<U>::wedgeUVs()
{
    return IteratorWrapper<Container, typename Container::iterator>(&vWedgeUVs);
}

template<class U>
IteratorWrapper<const typename MeshWedgeUVHandler<U>::Container, typename MeshWedgeUVHandler<U>::Container::const_iterator> MeshWedgeUVHandler<U>::wedgeUVs() const
{
    return IteratorWrapper<const Container, typename Container::const_iterator>(&vWedgeUVs);
}

template<class U>
void MeshWedgeUVHandler<U>::clearWedgeUVs()
{
    vWedgeUVs.clear();
}

}
