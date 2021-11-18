#include "mesh_smoothing.h"

#include <nvl/models/algorithms/mesh_adjacencies.h>

#include <nvl/math/comparisons.h>

namespace nvl {

/**
 * @brief Laplacian smoothing on mesh, given a constant alpha for each vertex
 * @param mesh Mesh
 * @param iterations Number of iterations
 * @param alpha Constant alpha for each vertex. A value between 0 and 1, it
 * represents the weight of the original coordinates
 */
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const Index iterations,
        const double alpha)
{
    const std::vector<std::vector<Index>> vvAdj = meshVertexVertexAdjacencies(mesh);
    meshLaplacianSmoothing(mesh, iterations, alpha, vvAdj);
}

/**
 * @brief Laplacian smoothing on mesh, given a constant alpha for each vertex
 * @param mesh Mesh
 * @param vvAdj Vertex-vertex adjacencies of the mesh
 * @param iterations Number of iterations
 * @param alpha Constant alpha for each vertex. A value between 0 and 1, it
 * represents the weight of the original coordinates
 */
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const Index iterations,
        const double alpha,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    assert(alpha >= 0.0 && alpha <= 1.0);

    if (epsEqual(alpha, 1.0))
        return;

    std::vector<Point> pointVector(mesh.nextVertexId());

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        pointVector[vId] = mesh.vertexPoint(vId);
    }

    for (Index it = 0; it < iterations; ++it) {
        std::vector<Point> tmpVector = pointVector;

        #pragma omp parallel for
        for (VertexId vId = 0; vId < tmpVector.size(); ++vId) {
            if (mesh.isVertexDeleted(vId))
                continue;

            Point value = alpha * tmpVector[vId];
            double adjWeight = (1 - alpha) / vvAdj[vId].size();
            for (const VertexId& adj : vvAdj[vId]) {
                assert(adj != NULL_ID);
                value += adjWeight * tmpVector[adj];
            }
            pointVector[vId] = value;
        }
    }

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        mesh.setVertexPoint(vId, pointVector[vId]);
    }
}

/**
 * @brief Laplacian smoothing on mesh, given a alpha for each vertex
 * @param mesh Mesh
 * @param alphas Alpha for each vertex. A value between 0 and 1, it represents
 * the weight of the original coordinates
 * @param iterations Number of iterations
 */
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const Index iterations,
        const std::vector<double>& alphas)
{
    const std::vector<std::vector<Index>> vvAdj = meshVertexVertexAdjacencies(mesh);
    meshLaplacianSmoothing(mesh, iterations, alphas, vvAdj);
}

/**
 * @brief Laplacian smoothing on mesh, given a alpha for each vertex
 * @param mesh Mesh
 * @param alphas Alpha for each vertex. A value between 0 and 1, it represents
 * @param vvAdj Vertex-vertex adjacencies of the mesh
 * the weight of the original coordinates
 * @param iterations Number of iterations
 */
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const Index iterations,
        const std::vector<double>& alphas,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    std::vector<Point> pointVector(mesh.nextVertexId());

    assert(alphas.size() == mesh.nextVertexId());

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        pointVector[vId] = mesh.vertexPoint(vId);
    }

    for (Index it = 0; it < iterations; ++it) {
        std::vector<Point> tmpVector = pointVector;

        #pragma omp parallel for
        for (VertexId vId = 0; vId < tmpVector.size(); ++vId) {
            if (mesh.isVertexDeleted(vId))
                continue;

            assert(alphas[vId] >= 0.0 && alphas[vId] <= 1.0);

            if (!epsEqual(alphas[vId], 1.0)) {
                continue;

                Point value = alphas[vId] * tmpVector[vId];
                double adjWeight = (1 - alphas[vId]) / vvAdj[vId].size();
                for (const VertexId& adj : vvAdj[vId]) {
                    assert(adj != NULL_ID);
                    value += adjWeight * tmpVector[adj];
                }
                pointVector[vId] = value;
            }
        }
    }

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        if (!epsEqual(alphas[vId], 1.0)) {
            mesh.setVertexPoint(vId, pointVector[vId]);
        }
    }
}

/**
 * @brief Laplacian smoothing on mesh subset, given a constant alpha for each vertex
 * @param mesh Mesh
 * @param vertices Vertices to be smoothed
 * @param iterations Number of iterations
 * @param alpha Constant alpha for each vertex. A value between 0 and 1, it
 * represents the weight of the original coordinates
 */
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const Index iterations,
        const double alpha)
{
    const std::vector<std::vector<Index>> vvAdj = meshVertexVertexAdjacencies(mesh);
    meshLaplacianSmoothing(mesh, vertices, iterations, alpha, vvAdj);
}

/**
 * @brief Laplacian smoothing on mesh subset, given a constant alpha for each vertex
 * @param mesh Mesh
 * @param vertices Vertices to be smoothed
 * @param iterations Number of iterations
 * @param vvAdj Vertex-vertex adjacencies of the mesh
 * @param alpha Constant alpha for each vertex. A value between 0 and 1, it
 * represents the weight of the original coordinates
 */
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const Index iterations,
        const double alpha,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    assert(alpha >= 0.0 && alpha <= 1.0);

    if (epsEqual(alpha, 1.0))
        return;

    std::vector<Point> pointVector(mesh.nextVertexId());

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        pointVector[vId] = mesh.vertexPoint(vId);
    }

    for (Index it = 0; it < iterations; ++it) {
        std::vector<Point> tmpVector = pointVector;

        #pragma omp parallel for
        for (Index i = 0; i < vertices.size(); i++) {
            const VertexId& vId = vertices[i];

            Point value = alpha * tmpVector[vId];
            double adjWeight = (1 - alpha) / vvAdj[vId].size();
            for (const VertexId& adj : vvAdj[vId]) {
                assert(adj != NULL_ID);
                value += adjWeight * tmpVector[adj];
            }
            pointVector[vId] = value;
        }
    }

    #pragma omp parallel for
    for (VertexId i = 0; i < vertices.size(); i++) {
        const VertexId& vId = vertices[i];
        mesh.setVertexPoint(vId, pointVector[vId]);
    }
}

/**
 * @brief Laplacian smoothing on mesh subset, given a constant alpha for each vertex
 * @param mesh Mesh
 * @param vertices Vertices to be smoothed
 * @param alphas Alpha for each vertex. A value between 0 and 1, it represents
 * the weight of the original coordinates
 * @param iterations Number of iterations
 */
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const Index iterations,
        const std::vector<double>& alphas)
{
    const std::vector<std::vector<Index>> vvAdj = meshVertexVertexAdjacencies(mesh);
    meshLaplacianSmoothing(mesh, vertices, iterations, alphas, vvAdj);
}

/**
 * @brief Laplacian smoothing on mesh subset, given a constant alpha for each vertex
 * @param mesh Mesh
 * @param vertices Vertices to be smoothed
 * @param alphas Alpha for each vertex. A value between 0 and 1, it represents
 * the weight of the original coordinates
 * @param iterations Number of iterations
 * @param vvAdj Vertex-vertex adjacencies of the mesh
 */
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,        
        const Index iterations,
        const std::vector<double>& alphas,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;


    std::vector<Point> pointVector(mesh.nextVertexId());

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        pointVector[vId] = mesh.vertexPoint(vId);
    }

    for (Index it = 0; it < iterations; ++it) {
        std::vector<Point> tmpVector = pointVector;

        #pragma omp parallel for
        for (Index i = 0; i < vertices.size(); i++) {
            const VertexId& vId = vertices[i];
            const double& alpha = alphas[i];

            assert(alpha >= 0.0 && alpha <= 1.0);

            if (!epsEqual(alpha, 1.0)) {
                Point value = alpha * tmpVector[vId];
                double adjWeight = (1 - alpha) / vvAdj[vId].size();
                for (const VertexId& adj : vvAdj[vId]) {
                    assert(adj != NULL_ID);
                    value += adjWeight * tmpVector[adj];
                }
                pointVector[vId] = value;
            }
        }
    }

    #pragma omp parallel for
    for (VertexId i = 0; i < vertices.size(); i++) {
        const VertexId& vId = vertices[i];
        const double& alpha = alphas[i];

        if (!epsEqual(alpha, 1.0)) {
            mesh.setVertexPoint(vId, pointVector[vId]);
        }
    }
}


}
