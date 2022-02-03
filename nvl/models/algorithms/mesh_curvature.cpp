/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_curvature.h"

#include <nvl/math/common_functions.h>
#include <nvl/math/angles.h>

#include <nvl/models/algorithms/mesh_operators.h>
#include <nvl/models/algorithms/mesh_borders.h>
#include <nvl/models/algorithms/mesh_geometric_information.h>
#include <nvl/models/algorithms/mesh_triangulation.h>
#include <nvl/models/algorithms/mesh_normals.h>

#ifdef NVL_VCGLIB_LOADED
#include <vcg/complex/algorithms/update/curvature.h>
#include <nvl/models/structures/vcg_triangle_mesh.h>
#include <nvl/models/algorithms/mesh_vcg_convert.h>
#endif

namespace nvl {

/**
 * @brief Calculate mean curvature using the default method (VCG or Laplace-Beltrami)
 * @param mesh Mesh
 * @return Mean curvature (per-vertex)
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> meshMeanCurvature(const Mesh& mesh)
{
#ifdef NVL_VCGLIB_LOADED
    return meshMeanCurvatureVCG(mesh);
#else
    return meshMeanCurvatureLB(mesh);
#endif
}

/**
 * @brief Calculate gaussian curvature using the default method (VCG or Laplace-Beltrami)
 * @param mesh Mesh
 * @return Gaussian curvature (per-vertex)
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> meshGaussianCurvature(const Mesh& mesh)
{
#ifdef NVL_VCGLIB_LOADED
    return meshGaussianCurvatureVCG(mesh);
#else
    return meshGaussianCurvatureLB(mesh);
#endif
}

#ifdef NVL_VCGLIB_LOADED

/**
 * @brief Calculate principal curvature using the default method (VCG)
 * @param mesh Mesh
 * @return Principal curvature directions (per-vertex)
 */
template<class Mesh>
std::vector<PrincipalCurvatures<typename Mesh::Scalar>> meshPrincipalCurvature(const Mesh& mesh)
{
    return meshPrincipalCurvatureVCG(mesh);
}

#endif

/**
 * @brief Calculate mean curvature (estimation using Laplace-Beltrami method)
 * @param mesh Mesh
 * @return Mean curvature (per-vertex)
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> meshMeanCurvatureLB(const Mesh& mesh)
{
    typedef typename Mesh::Scalar Scalar;

    Mesh triangulatedMesh = mesh;
    meshTriangulateConvexFace(triangulatedMesh);

    std::vector<Vector3<Scalar>> lbo = meshLaplaceBeltramiOperator(triangulatedMesh);

    std::vector<Scalar> mc(lbo.size());

    for (Index vId = 0; vId < lbo.size(); ++vId) {
        mc[vId] = 0.5 * abs(lbo[vId].norm());
    }

    return mc;
}

/**
 * @brief Calculate gaussian curvature (estimation using Laplace-Beltrami method)
 * @param mesh Mesh
 * @return Gaussian curvature (per-vertex)
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> meshGaussianCurvatureLB(const Mesh& mesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::Scalar Scalar;
    typedef typename Mesh::Point Point;

    Mesh triangulatedMesh = mesh;
    meshTriangulateConvexFace(triangulatedMesh);

    std::vector<std::vector<Index>> vfPos;
    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(triangulatedMesh);
    const std::vector<std::vector<FaceId>> vfAdj = meshVertexFaceAdjacencies(triangulatedMesh, vfPos);
    const std::vector<std::vector<FaceId>> ffAdj = meshFaceFaceAdjacencies(triangulatedMesh);

    std::vector<Scalar> gc(triangulatedMesh.nextVertexId());

    for (VertexId vId = 0; vId < triangulatedMesh.nextVertexId(); ++vId) {
        gc[vId] = 0;

        Scalar firstTerm;
        if (nvl::meshIsBorderVertex(triangulatedMesh, vId, ffAdj))
            firstTerm = M_PI;
        else
            firstTerm = M_PI * 2;

        Scalar area = 0.0;
        Scalar angleSum = 0.0;
        for (Index i = 0; i < vfAdj[vId].size(); ++i) {
            const FaceId& adjFId = vfAdj[vId][i];
            const Face& adjFace = triangulatedMesh.face(adjFId);

            const Index& pos = vfPos[vId][i];

            area += nvl::meshFaceArea(triangulatedMesh, adjFId);

            assert(adjFace.vertexId(pos) == vId);
            Point currentP = triangulatedMesh.vertexPoint(vId);
            Point nextP = triangulatedMesh.vertexPoint(adjFace.vertexId((pos + 1) % adjFace.vertexNumber()));
            Point prevP = triangulatedMesh.vertexPoint(adjFace.vertexId(pos > 0 ? pos - 1 : adjFace.vertexNumber() - 1));

            Vector3<Scalar> v1 = nextP - currentP;
            Vector3<Scalar> v2 = prevP - currentP;

            double a = angle(v1, v2);

            angleSum += a;

        }

        gc[vId] = (firstTerm - angleSum) / (area / 3.0);
    }

    return gc;
}

#ifdef NVL_VCGLIB_LOADED

/**
 * @brief Calculate mean curvature using VCG
 * @param mesh Mesh
 * @return Mean curvature (per-vertex)
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> meshMeanCurvatureVCG(const Mesh& mesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Scalar Scalar;

    std::vector<Scalar> gc(mesh.nextVertexId());

    Mesh triangulatedMesh = mesh;
    std::vector<FaceId> triangulatedBirthFace;
    meshTriangulateConvexFace(triangulatedMesh, triangulatedBirthFace);

    VCGTriangleMesh vcgMesh;

    std::vector<VertexId> vcgBirthVertex;
    std::vector<FaceId> vcgBirthFace;
    convertMeshToVCGMesh(triangulatedMesh, vcgMesh, vcgBirthVertex, vcgBirthFace);

    vcg::tri::UpdateTopology<VCGTriangleMesh>::FaceFace(vcgMesh);
    vcg::tri::UpdateCurvature<VCGTriangleMesh>::MeanAndGaussian(vcgMesh);

    for (size_t i = 0; i < vcgMesh.vert.size(); ++i) {
        if (vcgMesh.vert[i].IsD())
            continue;

        gc[vcgBirthVertex[i]] = vcgMesh.vert[i].Kh();
    }

    return gc;
}

/**
 * @brief Calculate gaussian curvature using VCG
 * @param mesh Mesh
 * @return Gaussian curvature (per-vertex)
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> meshGaussianCurvatureVCG(const Mesh& mesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Scalar Scalar;

    std::vector<Scalar> gc(mesh.nextVertexId());

    Mesh triangulatedMesh = mesh;
    std::vector<FaceId> triangulatedBirthFace;
    meshTriangulateConvexFace(triangulatedMesh, triangulatedBirthFace);

    VCGTriangleMesh vcgMesh;

    std::vector<VertexId> vcgBirthVertex;
    std::vector<FaceId> vcgBirthFace;
    convertMeshToVCGMesh(triangulatedMesh, vcgMesh, vcgBirthVertex, vcgBirthFace);

    vcg::tri::UpdateTopology<VCGTriangleMesh>::FaceFace(vcgMesh);
    vcg::tri::UpdateCurvature<VCGTriangleMesh>::MeanAndGaussian(vcgMesh);

    for (size_t i = 0; i < vcgMesh.vert.size(); ++i) {
        if (vcgMesh.vert[i].IsD())
            continue;

        gc[vcgBirthVertex[i]] = vcgMesh.vert[i].Kg();
    }

    return gc;
}

/**
 * @brief Calculate principal curvature directions using VCG
 * @param mesh Mesh
 * @return Principal curvature directions (per-vertex)
 */
template<class Mesh>
std::vector<PrincipalCurvatures<typename Mesh::Scalar>> meshPrincipalCurvatureVCG(const Mesh& mesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Scalar Scalar;

    std::vector<Scalar> gc(mesh.nextVertexId());

    Mesh triangulatedMesh = mesh;
    std::vector<FaceId> triangulatedBirthFace;
    meshTriangulateConvexFace(triangulatedMesh, triangulatedBirthFace);

    VCGTriangleMesh vcgMesh;

    std::vector<VertexId> vcgBirthVertex;
    std::vector<FaceId> vcgBirthFace;
    convertMeshToVCGMesh(triangulatedMesh, vcgMesh, vcgBirthVertex, vcgBirthFace);

    vcg::tri::UpdateTopology<VCGTriangleMesh>::FaceFace(vcgMesh);
    vcg::tri::UpdateTopology<VCGTriangleMesh>::VertexFace(vcgMesh);
    vcg::tri::UpdateCurvature<VCGTriangleMesh>::PrincipalDirections(vcgMesh);

    std::vector<PrincipalCurvatures<Scalar>> pc(mesh.nextVertexId());

    for (size_t i = 0; i < vcgMesh.vert.size(); ++i) {
        if (vcgMesh.vert[i].IsD())
            continue;

        pc[vcgBirthVertex[i]].d1 = Vector3<Scalar>(
                    vcgMesh.vert[i].PD1().X(),
                    vcgMesh.vert[i].PD1().Y(),
                    vcgMesh.vert[i].PD1().Z());
        pc[vcgBirthVertex[i]].d2 = Vector3<Scalar>(
                    vcgMesh.vert[i].PD2().X(),
                    vcgMesh.vert[i].PD2().Y(),
                    vcgMesh.vert[i].PD2().Z());

        pc[vcgBirthVertex[i]].k1 = vcgMesh.vert[i].K1();
        pc[vcgBirthVertex[i]].k2 = vcgMesh.vert[i].K2();
    }

    return pc;
}

#endif

}
