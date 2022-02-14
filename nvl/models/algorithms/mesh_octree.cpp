/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_octree.h"

#include <nvl/math/numeric_limits.h>
#include <nvl/math/closest_point.h>
#include <nvl/models/algorithms/mesh_geometric_information.h>
#include <nvl/models/algorithms/mesh_adjacencies.h>

namespace nvl {

/**
 * @brief Get an octree storing the vertices of a mesh
 * @param mesh Mesh
 * @param maxBoxElements Max elements in each box of the octree
 * @param maxBoxRadius Max radius of each box of the octree
 * @return Vertex octree
 */
template<class Mesh>
Octree<typename Mesh::Point, typename Mesh::VertexId> meshVertexOctree(
        const Mesh& mesh,
        const Size maxBoxElements,
        const typename Mesh::Scalar maxBoxRadius)
{
    typedef typename Mesh::VertexId VertexId;

    AlignedBox3<typename Mesh::Scalar> bbox = meshBoundingBox(mesh);
    Octree<typename Mesh::Point, typename Mesh::VertexId> o(bbox.center(), bbox.diagonal().norm());
    o.setMaxBoxElements(maxBoxElements);
    o.setMaxBoxRadius(maxBoxRadius);

    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        o.insert(mesh.vertexPoint(vId), vId);
    }

    return o;
}

/**
 * @brief Closest vertex in a mesh using an octree
 * @param mesh Mesh
 * @param octree Octree
 * @param point Point
 * @return Closest vertex id
 */
template<class Mesh>
typename Mesh::VertexId meshVertexOctreeClosestVertex(
        const Mesh& mesh,
        const Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const typename Mesh::Point& point)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;
    typedef typename Mesh::Scalar Scalar;
    typedef typename Octree<typename Mesh::Point, typename Mesh::VertexId>::Node Node;

    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);

    VertexId closestVId = NULL_ID;

    const Node* node = octree.findClosest(point);

    if (node == nullptr)
        return closestVId;

    Scalar closestDistance = nvl::maxLimitValue<Scalar>();
    for (const std::pair<Point, VertexId>& v : node->values) {
        const Point& p = v.first;

        Scalar dist = (point - p).norm();

        if (dist < closestDistance) {
            closestDistance = dist;
            closestVId = v.second;
        }
    }
    for (const Node* neighbor : node->neighbors()) {
        for (const std::pair<Point, VertexId>& v : neighbor->values) {
            const Point& p = v.first;

            Scalar dist = (point - p).norm();

            if (dist < closestDistance) {
                closestDistance = dist;
                closestVId = v.second;
            }
        }
    }

    return closestVId;
}

/**
 * @brief Closest point in a mesh using an octree
 * @param mesh Mesh
 * @param octree Octree
 * @param point Point
 * @return Closest point
 */
template<class Mesh>
typename Mesh::Point meshVertexOctreeClosestPoint(
        const Mesh& mesh,
        const Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const typename Mesh::Point& point)
{
    return meshVertexOctreeClosestPoint(mesh, octree, nvl::meshVertexFaceAdjacencies(mesh), point);
}

/**
 * @brief Closest point in a mesh using an octree
 * @param mesh Mesh
 * @param octree Octree
 * @param point Point
 * @param vfAdj Pre-computed vertex-face adjacencies
 * @return Closest point
 */
template<class Mesh>
typename Mesh::Point meshVertexOctreeClosestPoint(
        const Mesh& mesh,
        const Octree<typename Mesh::Point, typename Mesh::VertexId>& octree,
        const typename Mesh::Point& point,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::Point Point;
    typedef typename Mesh::Scalar Scalar;

    Point closestPoint;

    VertexId closestVId = meshVertexOctreeClosestVertex(mesh, octree, point);

    if (closestVId != NULL_ID) {
        Scalar bestClosestDistance = nvl::maxLimitValue<Scalar>();
        for (const FaceId& fId : vfAdj[closestVId]) {
            const Face& face = mesh.face(fId);

            Point p;
            if (face.vertexNumber() == 3) {
                p = closestPointOnTriangle(
                    mesh.vertexPoint(face.vertexId(0)),
                    mesh.vertexPoint(face.vertexId(1)),
                    mesh.vertexPoint(face.vertexId(2)),
                    point);
            }
            else {
                std::vector<Point> polygon(face.vertexNumber());
                for (Index j = 0; j < face.vertexNumber(); ++j) {
                    polygon[j] = mesh.vertexPoint(face.vertexId(j));
                }

                p = closestPointOnPolygonBarycenterSubdivision(polygon, point);
            }

            Scalar dist = (point - p).norm();
            if (dist < bestClosestDistance) {
                closestPoint = p;
                bestClosestDistance = dist;
            }
        }
    }

    return closestPoint;
}

}
