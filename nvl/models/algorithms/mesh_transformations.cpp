#include "mesh_transformations.h"

namespace nvl {

template<class Mesh, class T>
void meshApplyTransformation(Mesh& mesh, const T& transformation)
{
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::VertexId VertexId;

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (!mesh.isVertexDeleted(vId)) {
            Vertex& vertex = mesh.vertex(vId);
            vertex.setPoint(transformation * vertex.point());
        }
    }
}

template<class Mesh, class T>
void meshApplyTransformation(Mesh& mesh, const Matrix44<T>& transformation)
{
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (!mesh.isVertexDeleted(vId)) {
            Vertex& vertex = mesh.vertex(vId);
            Point p3 = vertex.point();

            Vector4<T> p4(p3.x(), p3.y(), p3.z(), 1);
            p4 = transformation * p4;
            p3 = Point(p4.x(), p4.y(), p4.z());

            vertex.setPoint(p3);
        }
    }
}

}