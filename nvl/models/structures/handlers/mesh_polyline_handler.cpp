/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_polyline_handler.h"

namespace nvl {

template<class T>
MeshPolylineHandler<T>::MeshPolylineHandler()
{

}

template<class T>
Size MeshPolylineHandler<T>::polylineNumber() const
{
    return vPolylines.size();
}

template<class T>
typename MeshPolylineHandler<T>::PolylineId MeshPolylineHandler<T>::nextPolylineId() const
{
    return vPolylines.realSize();
}

template<class T>
typename MeshPolylineHandler<T>::Polyline& MeshPolylineHandler<T>::polyline(const PolylineId& id)
{
    return vPolylines[id];
}

template<class T>
const typename MeshPolylineHandler<T>::Polyline& MeshPolylineHandler<T>::polyline(const PolylineId& id) const
{
    return vPolylines[id];
}

template<class T>
typename MeshPolylineHandler<T>::PolylineId MeshPolylineHandler<T>::addPolyline(const Polyline& polyline)
{
    assert(polyline.vertexNumber() >= 2);
    PolylineId newId = nextPolylineId();
    vPolylines.push_back(polyline);
    vPolylines[newId].setId(newId);
    return newId;
}

template<class T>
template<class... Ts>
typename MeshPolylineHandler<T>::PolylineId MeshPolylineHandler<T>::addPolyline(const Ts... vertices)
{
    PolylineId newId = nextPolylineId();
    Polyline polyline(vertices...);
    assert(polyline.vertexNumber() >= 2);
    polyline.setId(newId);
    vPolylines.push_back(polyline);
    return newId;
}

template<class T>
typename MeshPolylineHandler<T>::PolylineId MeshPolylineHandler<T>::allocatePolylines(const Size& n)
{
    return allocatePolylines(n, Polyline());
}

template<class T>
typename MeshPolylineHandler<T>::PolylineId MeshPolylineHandler<T>::allocatePolylines(const Size& n, const Polyline& polyline)
{
    Index firstIndex = vPolylines.realSize();
    Index lastIndex = firstIndex + n;

    vPolylines.resize(lastIndex, polyline);
    for (Index i = firstIndex; i < lastIndex; i++) {
        vPolylines[i].setId(i);
    }

    return firstIndex;
}

template<class T>
void MeshPolylineHandler<T>::deletePolyline(const PolylineId& id)
{
    Index copyId = id;
    vPolylines[copyId].setId(NULL_ID);
    vPolylines.erase(copyId);
}

template<class T>
bool MeshPolylineHandler<T>::isPolylineDeleted(const PolylineId& id) const
{
    return vPolylines.isDeleted(id);
}

template<class T>
void MeshPolylineHandler<T>::deletePolyline(const Polyline& polyline)
{
    deletePolyline(polyline.id());
}

template<class T>
bool MeshPolylineHandler<T>::isPolylineDeleted(const Polyline& polyline) const
{
    return isPolylineDeleted(polyline.id());
}

template<class T>
IteratorWrapper<VectorWithDelete<typename MeshPolylineHandler<T>::Polyline>, typename VectorWithDelete<typename MeshPolylineHandler<T>::Polyline>::iterator> MeshPolylineHandler<T>::polylines()
{
    return IteratorWrapper<VectorWithDelete<Polyline>, typename VectorWithDelete<Polyline>::iterator>(&vPolylines);
}

template<class T>
IteratorWrapper<const VectorWithDelete<typename MeshPolylineHandler<T>::Polyline>, typename VectorWithDelete<typename MeshPolylineHandler<T>::Polyline>::const_iterator> MeshPolylineHandler<T>::polylines() const
{
    return IteratorWrapper<const VectorWithDelete<Polyline>, typename VectorWithDelete<Polyline>::const_iterator>(&vPolylines);
}

template<class T>
void MeshPolylineHandler<T>::clearPolylines()
{
    vPolylines.clear();
}

}
