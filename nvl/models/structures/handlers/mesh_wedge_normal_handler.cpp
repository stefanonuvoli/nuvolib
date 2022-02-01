/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_wedge_normal_handler.h"

namespace nvl {

template<class N>
MeshWedgeNormalHandler<N>::MeshWedgeNormalHandler()
{

}

template<class N>
Size MeshWedgeNormalHandler<N>::wedgeNormalNumber() const
{
    return vWedgeNormals.size();
}

template<class N>
typename MeshWedgeNormalHandler<N>::WedgeNormalId MeshWedgeNormalHandler<N>::nextWedgeNormalId() const
{
    return vWedgeNormals.realSize();
}

template<class N>
typename MeshWedgeNormalHandler<N>::VertexNormal& MeshWedgeNormalHandler<N>::wedgeNormal(const WedgeNormalId& id)
{
    return vWedgeNormals[id];
}

template<class N>
const typename MeshWedgeNormalHandler<N>::VertexNormal& MeshWedgeNormalHandler<N>::wedgeNormal(const WedgeNormalId& id) const
{
    return vWedgeNormals[id];
}

template<class N>
typename MeshWedgeNormalHandler<N>::WedgeNormalId MeshWedgeNormalHandler<N>::addWedgeNormal(const VertexNormal& wedgeNormal)
{
    WedgeNormalId newId = nextWedgeNormalId();
    vWedgeNormals.push_back(wedgeNormal);
    return newId;
}

template<class N>
typename MeshWedgeNormalHandler<N>::WedgeNormalId MeshWedgeNormalHandler<N>::allocateWedgeNormals(const Size& n)
{
    return allocateWedgeNormals(n, VertexNormal());
}

template<class N>
typename MeshWedgeNormalHandler<N>::WedgeNormalId MeshWedgeNormalHandler<N>::allocateWedgeNormals(const Size& n, const VertexNormal& wedgeNormal)
{
    Index firstIndex = vWedgeNormals.realSize();
    Index lastIndex = firstIndex + n;

    vWedgeNormals.resize(lastIndex, wedgeNormal);

    return firstIndex;
}

template<class N>
void MeshWedgeNormalHandler<N>::deleteWedgeNormal(const WedgeNormalId& id)
{
    Size currentId = id;
    vWedgeNormals.erase(currentId);
}

template<class N>
bool MeshWedgeNormalHandler<N>::isWedgeNormalDeleted(const WedgeNormalId& id) const
{
    return vWedgeNormals.isDeleted(id);
}

template<class N>
IteratorWrapper<typename MeshWedgeNormalHandler<N>::Container, typename MeshWedgeNormalHandler<N>::Container::iterator> MeshWedgeNormalHandler<N>::wedgeNormals()
{
    return IteratorWrapper<Container, typename Container::iterator>(&vWedgeNormals);
}

template<class N>
IteratorWrapper<const typename MeshWedgeNormalHandler<N>::Container, typename MeshWedgeNormalHandler<N>::Container::const_iterator> MeshWedgeNormalHandler<N>::wedgeNormals() const
{
    return IteratorWrapper<const Container, typename Container::const_iterator>(&vWedgeNormals);
}

template<class N>
void MeshWedgeNormalHandler<N>::clearWedgeNormals()
{
    vWedgeNormals.clear();
}

}
