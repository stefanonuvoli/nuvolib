/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_consistency.h"

namespace nvl {

/**
 * @brief Remove faces with deleted vertices
 * @param mesh Mesh
 * @return Deleted faces
 */
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

/**
 * @brief Remove polylines with deleted vertices
 * @param mesh Mesh
 * @return Deleted polylines
 */
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
