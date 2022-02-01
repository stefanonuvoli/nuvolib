/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_geometric_information.h"

#include <vector>

#include <nvl/math/barycenter.h>

namespace nvl {

/**
 * @brief Axis-aligned bounding box of a mesh
 * @param mesh Mesh
 * @return Axis-aligned bounding box
 */
template<class Mesh>
AlignedBox3<typename Mesh::Scalar> meshBoundingBox(const Mesh& mesh)
{
    typedef typename Mesh::Scalar Scalar;
    typedef typename Mesh::Vertex Vertex;

    AlignedBox3<Scalar> boundingBox;

    for (const Vertex& vertex : mesh.vertices()) {
        boundingBox.extend(vertex.point());
    }

    return boundingBox;
}

/**
 * @brief Average edge length of a mesh
 * @param mesh Mesh
 * @return Average edge length
 */
template<class Mesh>
typename Mesh::Scalar meshAverageEdgeLength(const Mesh& mesh)
{
    typedef typename Mesh::Scalar Scalar;
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;

    Scalar length = 0.0;
    unsigned int n = 0;

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        for (Index j = 0; j < face.vertexNumber(); ++j) {
            const Vertex& v1 = mesh.vertex(face.vertexId(j));
            const Vertex& v2 = mesh.vertex(face.nextVertexId(j));

            length += (v1.point() - v2.point()).norm();
            n++;
        }
    }

    return length / n;
}

/**
 * @brief Barycenter of a face of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @return Barycenter
 */
template<class Mesh>
typename Mesh::Point meshFaceBarycenter(const Mesh& mesh, const typename Mesh::FaceId& fId)
{
    return meshFaceBarycenter(mesh, mesh.face(fId));
}

/**
 * @brief Barycenter of a face of a mesh
 * @param mesh Mesh
 * @param face Face
 * @return Barycenter
 */
template<class Mesh>
typename Mesh::Point meshFaceBarycenter(const Mesh& mesh, const typename Mesh::Face& face)
{
    typedef typename Mesh::Point Point;

    std::vector<Point> points(face.vertexNumber());

    for (Index j = 0; j < face.vertexNumber(); ++j) {
        points[j] = mesh.vertexPoint(face.vertexId(j));
    }

    return barycenter(points);
}

/**
 * @brief Midpoint of an edge of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @param fePos Position of the edge in the face
 * @return Midpoint of the edge
 */
template<class Mesh>
typename Mesh::Point meshFaceEdgeMidpoint(const Mesh& mesh, const typename Mesh::FaceId& fId, const Index& fePos)
{
    return meshFaceEdgeMidpoint(mesh, mesh.face(fId), fePos);
}

/**
 * @brief Midpoint of an edge of a mesh
 * @param mesh Mesh
 * @param face Face
 * @param fePos Position of the edge in the face
 * @return Midpoint of the edge
 */
template<class Mesh>
typename Mesh::Point meshFaceEdgeMidpoint(const Mesh& mesh, const typename Mesh::Face& face, const Index& fePos)
{
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexId VertexId;

    const VertexId& v1 = face.vertexId(fePos);
    const VertexId& v2 = face.nextVertexId(fePos);
    Point p = (mesh.vertexPoint(v1) + mesh.vertexPoint(v2)) / 2;

    return p;
}

/**
 * @brief Area of a face of a mesh
 * @param mesh Mesh
 * @param fId Face id
 * @return Area of the face
 */
template<class Mesh>
typename Mesh::Scalar meshFaceArea(const Mesh& mesh, const typename Mesh::FaceId& fId)
{
    return meshFaceArea(mesh, mesh.face(fId));
}

/**
 * @brief Area of a face of a mesh
 * @param mesh Mesh
 * @param face Face
 * @return Area of the face
 */
template<class Mesh>
typename Mesh::Scalar meshFaceArea(const Mesh& mesh, const typename Mesh::Face& face)
{
    typedef typename Mesh::Point Point;
    typedef typename Mesh::Scalar Scalar;

    Point barycenter = meshFaceBarycenter(mesh, face);

    Scalar area = 0;

    for (Index j = 0; j < face.vertexNumber(); ++j) {
        const Point& p1 = mesh.vertexPoint(face.vertexId(j));
        const Point& p2 = mesh.vertexPoint(face.nextVertexId(j));

        Point v1 = p2 - p1;
        Point v2 = barycenter - p1;

        area += 0.5 * (v1.cross(v2)).norm();
    }

    return area;
}

}
