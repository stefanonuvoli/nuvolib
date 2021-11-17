#include "mesh_geometric_information.h"

#include <vector>

#include <nvl/math/barycenter.h>

namespace nvl {

template<class Mesh>
AlignedBox3<typename Mesh::Scalar> meshBoundingBox(const Mesh &mesh)
{
    typedef typename Mesh::Scalar Scalar;
    typedef typename Mesh::Vertex Vertex;

    AlignedBox3<Scalar> boundingBox;

    for (const Vertex& vertex : mesh.vertices()) {
        boundingBox.extend(vertex.point());
    }

    return boundingBox;
}

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

template<class Mesh>
typename Mesh::Point meshFaceBarycenter(const Mesh& mesh, const typename Mesh::FaceId& fId)
{
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexId VertexId;

    std::vector<Point> points;

    for (const VertexId& vId : mesh.faceVertexIds(fId)) {
        points.push_back(mesh.vertexPoint(vId));
    }

    return barycenter(points);
}

template<class Mesh>
typename Mesh::Point meshFaceBarycenter(const Mesh& mesh, const typename Mesh::Face& face)
{
    return meshFaceBarycenter(mesh, face.id());
}

template<class Mesh>
typename Mesh::Point meshFaceEdgeMidpoint(const Mesh& mesh, const typename Mesh::FaceId& fId, const Index& fePos)
{
    typedef typename Mesh::Point Point;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::VertexId VertexId;

    const Face& face = mesh.face(fId);

    const VertexId& v1 = face.vertexId(fePos);
    const VertexId& v2 = face.nextVertexId(fePos);
    Point p = (mesh.vertexPoint(v1) + mesh.vertexPoint(v2)) / 2;

    return p;
}

template<class Mesh>
typename Mesh::Point meshFaceEdgeMidpoint(const Mesh& mesh, const typename Mesh::Face& face, const Index& fePos)
{
    return meshFaceEdgeMidpoint(mesh, face.id(), fePos);
}

}
