/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_curvature.h"

#include <nvl/math/common_functions.h>
#include <nvl/math/angles.h>

#include <nvl/models/algorithms/mesh_borders.h>
#include <nvl/models/algorithms/mesh_geometric_information.h>

namespace nvl {

/**
 * @brief Calculate mean curvature using Laplace-Beltrami
 * @param mesh Mesh
 * @return Mean curvature (per-vertex)
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> meshMeanCurvatureLB(const Mesh& mesh)
{
    typedef typename Mesh::Scalar Scalar;

    std::vector<Vector3<Scalar>> lbo = meshLaplaceBeltramiOperator(mesh);

    std::vector<Scalar> mc(lbo.size());

    for (Index vId = 0; vId < lbo.size(); ++vId) {
        mc[vId] = 0.5 * abs(lbo[vId].norm());
    }

    return mc;
}

/**
 * @brief Calculate gaussian curvature using Laplace-Beltrami
 * @param mesh Mesh
 * @return Mean curvature (per-vertex)
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> meshGaussianCurvature(const Mesh& mesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::Scalar Scalar;
    typedef typename Mesh::Point Point;

    std::vector<std::vector<Index>> vfPos;
    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    const std::vector<std::vector<FaceId>> vfAdj = meshVertexFaceAdjacencies(mesh, vfPos);
    const std::vector<std::vector<FaceId>> ffAdj = meshFaceFaceAdjacencies(mesh);

    std::vector<Scalar> gc(mesh.nextVertexId());

    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        gc[vId] = 0;

        Scalar firstTerm;
        if (nvl::meshIsBorderVertex(mesh, vId, ffAdj))
            firstTerm = M_PI;
        else
            firstTerm = M_PI * 2;

        Scalar area = 0.0;
        Scalar angleSum = 0.0;
        for (Index i = 0; i < vfAdj[vId].size(); ++i) {
            const FaceId& adjFId = vfAdj[vId][i];
            const Face& adjFace = mesh.face(adjFId);

            const Index& pos = vfPos[vId][i];

            area += nvl::meshFaceArea(mesh, adjFId);

            assert(adjFace.vertexId(pos) == vId);
            Point currentP = mesh.vertexPoint(vId);
            Point nextP = mesh.vertexPoint(adjFace.vertexId((pos+1) % adjFace.vertexNumber()));
            Point prevP = mesh.vertexPoint(adjFace.vertexId(pos > 0 ? pos-1 : adjFace.vertexNumber()-1));

            Vector3<Scalar> v1 = nextP - currentP;
            Vector3<Scalar> v2 = prevP - currentP;

            double a = angle(v1, v2);

            angleSum += a;

        }

        gc[vId] = (firstTerm - angleSum) / (area / 3.0);
    }

    return gc;
}

}
