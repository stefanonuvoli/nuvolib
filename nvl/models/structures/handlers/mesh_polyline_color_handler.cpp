/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_polyline_color_handler.h"

namespace nvl {

template<class P, class C>
MeshPolylineColorHandler<P,C>::MeshPolylineColorHandler()
{

}

template<class P, class C>
typename MeshPolylineColorHandler<P,C>::Color& MeshPolylineColorHandler<P,C>::polylineColor(const PolylineId& id)
{
    assert(id < vPolylineColors.realSize() && "Polyline colors not enabled or not properly initialized.");

    return vPolylineColors[id];
}

template<class P, class C>
const typename MeshPolylineColorHandler<P,C>::Color& MeshPolylineColorHandler<P,C>::polylineColor(const PolylineId& id) const
{
    assert(id < vPolylineColors.realSize() && "Polyline colors not enabled or not properly initialized.");

    return vPolylineColors[id];
}

template<class P, class C>
void MeshPolylineColorHandler<P,C>::setPolylineColor(const PolylineId& id, const Color& color)
{
    assert(id < vPolylineColors.realSize() && "Polyline colors not enabled or not properly initialized.");

    vPolylineColors[id] = color;
}

template<class P, class C>
typename MeshPolylineColorHandler<P,C>::Color& MeshPolylineColorHandler<P,C>::polylineColor(const Polyline& polyline)
{
    return polylineColor(polyline.id());
}

template<class P, class C>
const typename MeshPolylineColorHandler<P,C>::Color& MeshPolylineColorHandler<P,C>::polylineColor(const Polyline& polyline) const
{
    return polylineColor(polyline.id());
}

template<class P, class C>
void MeshPolylineColorHandler<P,C>::setPolylineColor(const Polyline& polyline, const Color& color)
{
    setPolylineColor(polyline.id(), color);
}

}
