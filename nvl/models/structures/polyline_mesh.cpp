#include "polyline_mesh.h"

namespace nvl {

template<class V, class L>
PolylineMesh<V,L>::PolylineMesh() :
    vPolylineColorsEnabled(false)
{
    this->vType = MeshType::POLYLINE;
}

template<class V, class L>
typename PolylineMesh<V,L>::PolylineId PolylineMesh<V,L>::addPolyline(const Polyline& polyline)
{
    PolylineId newPId = MeshPolylineHandler<L>::addPolyline(polyline);
    updateComponents();
    return newPId;
}

template<class V, class L>
template<class... Ts>
typename PolylineMesh<V,L>::PolylineId PolylineMesh<V,L>::addPolyline(const Ts... points)
{
    PolylineId newPId = MeshPolylineHandler<L>::addPolyline(points...);
    updateComponents();
    return newPId;
}

template<class V, class L>
typename PolylineMesh<V,L>::PolylineId PolylineMesh<V,L>::allocatePolylines(const Size& n)
{
    return allocatePolylines(n, Polyline());
}

template<class V, class L>
typename PolylineMesh<V,L>::PolylineId PolylineMesh<V,L>::allocatePolylines(const Size& n, const Polyline& polyline)
{
    PolylineId newPId = MeshPolylineHandler<L>::allocatePolylines(n, polyline);
    updateComponents();
    return newPId;
}

template<class V, class L>
typename L::VertexContainer& PolylineMesh<V,L>::polylineVertexIds(const PolylineId& id)
{
    return MeshPolylineHandler<L>::polyline(id).vertexIds();
}

template<class V, class L>
Size PolylineMesh<V,L>::polylineVertexNumber(const PolylineId& id) const
{
    MeshPolylineHandler<L>::polyline(id).vertexNumber();
}

template<class V, class L>
const typename L::VertexContainer& PolylineMesh<V,L>::polylineVertexIds(const PolylineId& id) const
{
    return MeshPolylineHandler<L>::polyline(id).vertexIds();
}

template<class V, class L>
void PolylineMesh<V,L>::setPolylineVertexIds(const PolylineId& id, const typename L::VertexContainer& vertexIds)
{
    MeshPolylineHandler<L>::polyline(id).setVertexIds(vertexIds);
}

template<class V, class L>
typename PolylineMesh<V,L>::VertexId PolylineMesh<V,L>::polylineVertexId(const PolylineId& id, const Index& pos)
{
    return MeshPolylineHandler<L>::polyline(id).vertexId(pos);
}

template<class V, class L>
const typename PolylineMesh<V,L>::VertexId PolylineMesh<V,L>::polylineVertexId(const PolylineId& id, const Index& pos) const
{
    return MeshPolylineHandler<L>::polyline(id).vertexId(pos);
}

template<class V, class L>
void PolylineMesh<V,L>::setPolylineVertexId(const PolylineId& id, const Index& pos, const VertexId& vId)
{
    MeshPolylineHandler<L>::polyline(id).setVertexId(pos, vId);
}

template<class V, class L>
bool PolylineMesh<V,L>::hasPolylineColors() const
{
    return vPolylineColorsEnabled;
}

template<class V, class L>
void PolylineMesh<V,L>::enablePolylineColors()
{
    vPolylineColorsEnabled = true;
    updatePolylineColors();
}

template<class V, class L>
void PolylineMesh<V,L>::disablePolylineColors()
{
    vPolylineColorsEnabled = false;
    updatePolylineColors();
}

template<class V, class L>
void PolylineMesh<V,L>::compactAll()
{
    compactPolylines();
    compactPolylines();
}

template<class V, class L>
std::vector<typename PolylineMesh<V,L>::PolylineId> PolylineMesh<V,L>::compactPolylines()
{
    std::vector<Index> pMap = this->vPolylines.compact();

    //Update polyline ids
    PolylineId newPolylineId = 0;
    for (Polyline& polyline : this->polylines()) {
        polyline.setId(newPolylineId);
        ++newPolylineId;
    }

    return pMap;
}

template<class V, class L>
std::vector<typename PolylineMesh<V,L>::VertexId> PolylineMesh<V,L>::compactVertices()
{
    std::vector<Index> vMap = VertexMesh<V>::compactVertices();

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


template<class V, class L>
void PolylineMesh<V,L>::clearPolylines()
{
    MeshPolylineHandler<L>::clearPolylines();
    this->vPolylineColors.clear();
}

template<class V, class L>
void PolylineMesh<V,L>::clear()
{
    VertexMesh<V>::clear();
    this->clearPolylines();
}

template<class V, class L>
void PolylineMesh<V,L>::updateComponents()
{
    updatePolylineColors();
}

template<class V, class L>
void PolylineMesh<V,L>::updatePolylineColors()
{
    if (vPolylineColorsEnabled) {
        this->vPolylineColors.resize(this->nextPolylineId(), VertexColor(0.7, 0.7, 0.7));
    }
    else {
        this->vPolylineColors.clear();
    }
}

template<class V, class L>
std::ostream& operator<<(std::ostream& output, const PolylineMesh<V,L>& polylineMesh)
{
    const VertexMesh<V>& vMesh = polylineMesh;
    output << vMesh;

    output << "Polylines: " << polylineMesh.polylineNumber() << std::endl;
    for (const typename PolylineMesh<V,L>::Polyline& polyline : polylineMesh.polylines()) {
        output << polyline << std::endl;
    }

    return output;
}

}
