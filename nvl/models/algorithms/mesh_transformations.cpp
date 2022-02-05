/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_transformations.h"

namespace nvl {

/**
 * @brief Apply transformation to a mesh
 * @param mesh Mesh
 * @param transformation Transformation
 */
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

/**
 * @brief Apply transformation to a mesh
 * @param mesh Mesh
 * @param transformation Transformation
 */
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
