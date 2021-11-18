#include "mesh_consistency.h"

namespace nvl {

template<class Mesh>
std::vector<typename Mesh::FaceId> meshRemoveFacesWithDeletedVertices(Mesh& mesh)
{
    std::vector<typename Mesh::FaceId> deletedFaces;

    for (const typename Mesh::Face& face : mesh.faces()) {
        for (const typename Mesh::VertexId& vId : face.vertexIds()) {
            if (vId >= mesh.nextVertexId() || mesh.isVertexDeleted(vId)) {
                deletedFaces.push_back(face.id());
                mesh.deleteFace(face);
                break;
            }
        }
    }

    return deletedFaces;
}

template<class Mesh>
std::vector<typename Mesh::PolylineId> meshRemovePolylinesWithDeletedVertices(Mesh& mesh)
{
    std::vector<typename Mesh::PolylineId> deletedPolylines;

    for (const typename Mesh::Polyline& polyline : mesh.polylines()) {
        for (const typename Mesh::VertexId& vId : polyline.vertexIds()) {
            if (vId >= mesh.nextVertexId() || mesh.isVertexDeleted(vId)) {
                deletedPolylines.push_back(polyline.id());
                mesh.deletePolyline(polyline);
                break;
            }
        }
    }

    return deletedPolylines;
}

}
