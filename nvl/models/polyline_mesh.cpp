#include "polyline_mesh.h"

namespace nvl {

template<class V, class L>
PolylineMesh<V,L>::PolylineMesh()
{
    this->vType = MeshType::POLYLINE;
}

template<class V, class L>
void PolylineMesh<V,L>::compactAll()
{
    compactVertices();
    compactPolylines();
}

template<class V, class L>
std::vector<typename PolylineMesh<V,L>::VertexId> PolylineMesh<V,L>::compactVertices()
{
    std::vector<Index> vMap = VertexMesh<V>::compactVertices();

    //Update vertex ids in polylines
    for (Polyline& polyline : this->polylines()) {
        for (Index i = 0; i < polyline.vertexNumber(); ++i) {
            VertexId oldId = polyline.vertexId(i);
            if (oldId != MAX_INDEX) {
                VertexId newId = vMap.at(oldId);
                polyline.setVertexId(i, newId);
            }
        }
    }

    return vMap;
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
void PolylineMesh<V,L>::clear()
{
    VertexMesh<V>::clear();
    this->clearPolylines();
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
