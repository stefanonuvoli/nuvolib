#include "mesh_geodesics.h"

#ifdef NVL_LIBIGL_LOADED

#include <nvl/models/algorithms/mesh_triangulation.h>
#include <nvl/models/algorithms/mesh_eigen_convert.h>

#include <nvl/math/numeric_limits.h>

#include <igl/exact_geodesic.h>

namespace nvl {

/**
 * @brief Get exact geodesics given source and target vertices
 * @param m Input mesh
 * @param sourceVertices Source vertices
 * @param targetVertices Target vertices
 * @return Output geodesics for each target vertex
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> exactGeodesics(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& sourceVertices,
        const std::vector<typename Mesh::VertexId>& targetVertices)
{
    typedef typename Mesh::VertexId VertexId;

    std::vector<VertexId> triBirthFace;

    Mesh triangulatedMesh = mesh;
    meshTriangulateConvexFace(triangulatedMesh, triBirthFace);

    std::vector<VertexId> conBirthVertex;
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    convertMeshToEigenMesh(triangulatedMesh, V, F, conBirthVertex);

    std::vector<VertexId> conVertexMap = inverseFunction(conBirthVertex, triangulatedMesh.nextVertexId());

    std::vector<VertexId> conTargetVertices(targetVertices.size());
    for (Index i = 0; i < targetVertices.size(); i++) {
        assert(conVertexMap[targetVertices[i]] != NULL_ID);
        conTargetVertices[i] = conVertexMap[targetVertices[i]];
    }

    Eigen::VectorXi VS, FS, VT, FT;

    VS.resize(sourceVertices.size());
    for (Index i = 0; i < sourceVertices.size(); i++)
        VS(i) = sourceVertices[i];
    VT.resize(conTargetVertices.size());
    for (Index i = 0; i < conTargetVertices.size(); i++)
        VT(i) = conTargetVertices[i];

    Eigen::VectorXd d;

    igl::exact_geodesic(V, F, VS, FS, VT, FT, d);

    std::vector<typename Mesh::Scalar> vertexGeodesics(conTargetVertices.size());
    for (Index i = 0; i < conTargetVertices.size(); i++) {
        vertexGeodesics[i] = d(i);
    }

    return vertexGeodesics;
}


/**
 * @brief Get exact geodesics given source vertices
 * @param m Input mesh
 * @param sourceVertices Source vertices
 * @return Output geodesics for each vertex
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> exactGeodesics(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& sourceVertices)
{
    typedef typename Mesh::VertexId VertexId;

    std::vector<VertexId> targetVertices;
    for (Index vId = 0; vId < mesh.nextVertexId(); vId++) {
        if (mesh.isVertexDeleted(vId))
            continue;

        targetVertices.push_back(vId);
    }

    return exactGeodesics(mesh, sourceVertices, targetVertices);
}

/**
 * @brief Get heat geodesics precomputed data
 * @param m Input mesh
 * @param sourceVertices Source vertices
 */
template<class Mesh>
void heatGeodesicsPrecomputeData(
        const Mesh& mesh,
        HeatGeodesicsData<Mesh>& precomputedData)
{
    std::vector<typename Mesh::VertexId> triBirthFace;

    Mesh triangulatedMesh = mesh;
    meshTriangulateConvexFace(triangulatedMesh, triBirthFace);

    std::vector<typename Mesh::VertexId> conBirthVertex;
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    convertMeshToEigenMesh(triangulatedMesh, V, F, conBirthVertex);

    igl::heat_geodesics_precompute(V, F, precomputedData.data);

    precomputedData.birthVertex.resize(conBirthVertex.size());
    for (size_t i = 0; i < conBirthVertex.size(); i++) {
        assert(conBirthVertex[i] != MAX_ID);
        precomputedData.birthVertex[i] = conBirthVertex[i];
    }
}

/**
 * @brief Get heat geodesics approximation given source vertices
 * @param m Input mesh
 * @param sourceVertices Source vertices
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> heatGeodesics(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& sourceVertices)
{
    HeatGeodesicsData<Mesh> precomputedData;

    heatGeodesicsPrecomputeData(
        mesh,
        precomputedData);

    return heatGeodesics(
        mesh,
        precomputedData,
        sourceVertices);
}

/**
 * @brief Get heat geodesics approximation given source vertices
 * @param precomputedData Precomputed heat geodesics data
 * @param sourceVertices Source vertices
 * @return Geodesic values per vertex
 */
template<class Mesh>
std::vector<typename Mesh::Scalar> heatGeodesics(
        const Mesh& mesh,
        const HeatGeodesicsData<Mesh>& precomputedData,
        const std::vector<typename Mesh::VertexId>& sourceVertices)
{
    std::vector<typename Mesh::Scalar> geodesics;

    Eigen::VectorXi gamma;
    gamma.resize(sourceVertices.size());
    for (Index i = 0; i < sourceVertices.size(); i++) {
        gamma(i) = sourceVertices[i];
    }

    Eigen::VectorXd D;

    igl::heat_geodesics_solve(precomputedData.data, gamma, D);

    geodesics.resize(mesh.nextVertexId(), maxLimitValue<typename Mesh::Scalar>());
    for (int i = 0; i < D.size(); i++) {
        geodesics[precomputedData.birthVertex[i]] = D(i);
    }

    return geodesics;
}

}

#endif
