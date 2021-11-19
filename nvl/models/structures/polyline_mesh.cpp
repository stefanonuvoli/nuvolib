#include "polyline_mesh.h"

namespace nvl {

template<class VT, class PT>
PolylineMesh<VT,PT>::PolylineMesh() :
    vPolylineColorsEnabled(false)
{
    this->vType = MeshType::POLYLINE;
}

template<class VT, class PT>
typename PolylineMesh<VT,PT>::PolylineId PolylineMesh<VT,PT>::addPolyline(const Polyline& polyline)
{
    PolylineId newPId = MeshPolylineHandler<Polyline>::addPolyline(polyline);
    updateComponents();
    return newPId;
}

template<class VT, class PT>
template<class... Ts>
typename PolylineMesh<VT,PT>::PolylineId PolylineMesh<VT,PT>::addPolyline(const Ts... points)
{
    PolylineId newPId = MeshPolylineHandler<Polyline>::addPolyline(points...);
    updateComponents();
    return newPId;
}

template<class VT, class PT>
typename PolylineMesh<VT,PT>::PolylineId PolylineMesh<VT,PT>::allocatePolylines(const Size& n)
{
    return allocatePolylines(n, Polyline());
}

template<class VT, class PT>
typename PolylineMesh<VT,PT>::PolylineId PolylineMesh<VT,PT>::allocatePolylines(const Size& n, const Polyline& polyline)
{
    PolylineId newPId = MeshPolylineHandler<Polyline>::allocatePolylines(n, polyline);
    updateComponents();
    return newPId;
}

template<class VT, class PT>
typename PolylineMesh<VT,PT>::PolylineContainer& PolylineMesh<VT,PT>::polylineVertexIds(const PolylineId& id)
{
    return MeshPolylineHandler<Polyline>::polyline(id).vertexIds();
}

template<class VT, class PT>
Size PolylineMesh<VT,PT>::polylineVertexNumber(const PolylineId& id) const
{
    MeshPolylineHandler<Polyline>::polyline(id).vertexNumber();
}

template<class VT, class PT>
const typename PolylineMesh<VT,PT>::PolylineContainer& PolylineMesh<VT,PT>::polylineVertexIds(const PolylineId& id) const
{
    return MeshPolylineHandler<Polyline>::polyline(id).vertexIds();
}

template<class VT, class PT>
void PolylineMesh<VT,PT>::setPolylineVertexIds(const PolylineId& id, const PolylineContainer& vertexIds)
{
    MeshPolylineHandler<Polyline>::polyline(id).setVertexIds(vertexIds);
}

template<class VT, class PT>
typename PolylineMesh<VT,PT>::VertexId PolylineMesh<VT,PT>::polylineVertexId(const PolylineId& id, const Index& pos)
{
    return MeshPolylineHandler<Polyline>::polyline(id).vertexId(pos);
}

template<class VT, class PT>
const typename PolylineMesh<VT,PT>::VertexId PolylineMesh<VT,PT>::polylineVertexId(const PolylineId& id, const Index& pos) const
{
    return MeshPolylineHandler<Polyline>::polyline(id).vertexId(pos);
}

template<class VT, class PT>
void PolylineMesh<VT,PT>::setPolylineVertexId(const PolylineId& id, const Index& pos, const VertexId& vId)
{
    MeshPolylineHandler<Polyline>::polyline(id).setVertexId(pos, vId);
}

template<class VT, class PT>
bool PolylineMesh<VT,PT>::hasPolylineColors() const
{
    return vPolylineColorsEnabled;
}

template<class VT, class PT>
void PolylineMesh<VT,PT>::enablePolylineColors()
{
    vPolylineColorsEnabled = true;
    updatePolylineColors();
}

template<class VT, class PT>
void PolylineMesh<VT,PT>::disablePolylineColors()
{
    vPolylineColorsEnabled = false;
    updatePolylineColors();
}

template<class VT, class PT>
void PolylineMesh<VT,PT>::compactAll()
{
    compactPolylines();
    compactPolylines();
}

template<class VT, class PT>
std::vector<typename PolylineMesh<VT,PT>::PolylineId> PolylineMesh<VT,PT>::compactPolylines()
{
    //Compact polyline colors
    if (this->hasPolylineColors()) {
        for (PolylineId vId = 0; vId < this->nextPolylineId(); vId++) {
            if (this->isPolylineDeleted(vId)) {
                this->vPolylineColors.erase(vId);
            }
        }
        this->vPolylineColors.compact();
    }
    else {
        this->vPolylineColors.clear();
    }

    std::vector<Index> pMap = this->vPolylines.compact();

    //Update polyline ids
    PolylineId newPolylineId = 0;
    for (Polyline& polyline : this->polylines()) {
        polyline.setId(newPolylineId);
        ++newPolylineId;
    }

    return pMap;
}

template<class VT, class PT>
std::vector<typename PolylineMesh<VT,PT>::VertexId> PolylineMesh<VT,PT>::compactVertices()
{
    std::vector<Index> vMap = VertexMesh<VT>::compactVertices();

    //Update vertex ids in polylines
    for (Polyline& polyline : this->polylines()) {
        for (Index i = 0; i < polyline.vertexNumber(); ++i) {
            VertexId oldId = polyline.vertexId(i);
            if (oldId != NULL_ID) {
                VertexId newId = vMap.at(oldId);
                polyline.setVertexId(i, newId);
            }
        }
    }

    return vMap;
}


template<class VT, class PT>
void PolylineMesh<VT,PT>::clearPolylines()
{
    MeshPolylineHandler<Polyline>::clearPolylines();
    this->vPolylineColors.clear();
}

template<class VT, class PT>
void PolylineMesh<VT,PT>::clear()
{
    VertexMesh<VT>::clear();
    this->clearPolylines();
}

template<class VT, class PT>
void PolylineMesh<VT,PT>::updateComponents()
{
    updatePolylineColors();
}

template<class VT, class PT>
void PolylineMesh<VT,PT>::updatePolylineColors()
{
    if (vPolylineColorsEnabled) {
        this->vPolylineColors.resize(this->nextPolylineId(), VertexColor(0.7, 0.7, 0.7));
    }
    else {
        this->vPolylineColors.clear();
    }
}

template<class VT, class PT>
std::ostream& operator<<(std::ostream& output, const PolylineMesh<VT,PT>& polylineMesh)
{
    const VertexMesh<VT>& vMesh = polylineMesh;
    output << vMesh;

    output << "Polylines: " << polylineMesh.polylineNumber() << std::endl;
    for (const typename PolylineMesh<VT,PT>::Polyline& polyline : polylineMesh.polylines()) {
        output << polyline << std::endl;
    }

    return output;
}

}
