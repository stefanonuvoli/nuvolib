/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_smoothing.h"

#include <nvl/models/algorithms/mesh_adjacencies.h>
#include <nvl/models/algorithms/mesh_borders.h>

#include <nvl/math/cotangent_weights.h>
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
        const unsigned int iterations,
        const double alpha)
{
    typedef typename Mesh::VertexId VertexId;
    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
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
        const unsigned int iterations,
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

    for (unsigned int it = 0; it < iterations; ++it) {
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
        const unsigned int iterations,
        const std::vector<double>& alphas)
{
    typedef typename Mesh::VertexId VertexId;
    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    meshLaplacianSmoothing(mesh, iterations, alphas, vvAdj);
}

/**
 * @brief Laplacian smoothing on mesh, given a alpha for each vertex
 * @param mesh Mesh 
 * @param alphas Alpha for each vertex. A value between 0 and 1, it represents
 * the weight of the original coordinates
 * @param vvAdj Vertex-vertex adjacencies of the mesh
 * @param iterations Number of iterations
 */
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const unsigned int iterations,
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

    for (unsigned int it = 0; it < iterations; ++it) {
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
        const unsigned int iterations,
        const double alpha)
{
    typedef typename Mesh::VertexId VertexId;
    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
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
        const unsigned int iterations,
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

    for (unsigned int it = 0; it < iterations; ++it) {
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
        const unsigned int iterations,
        const std::vector<double>& alphas)
{
    typedef typename Mesh::VertexId VertexId;
    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
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
        const unsigned int iterations,
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

    for (unsigned int it = 0; it < iterations; ++it) {
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

/* Cotangent laplacian smoothing */

/**
 * @brief Cotangent weight laplacian smoothing
 * @param mesh Mesh
 * @param iterations Number of iterations
 * @param alpha Constant alpha for each vertex. A value between 0 and 1, it
 * represents the weight of the original coordinates
 */
template<class Mesh>
void meshCotangentSmoothing(
        Mesh& mesh,
        const unsigned int iterations,
        const double alpha)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    const std::vector<std::vector<FaceId>> vfAdj = meshVertexFaceAdjacencies(mesh);
    meshCotangentSmoothing(mesh, iterations, alpha, vvAdj, vfAdj);
}

/**
 * @brief Cotangent weight laplacian smoothing
 * @param mesh Mesh
 * @param iterations Number of iterations
 * @param alpha Constant alpha for each vertex. A value between 0 and 1, it
 * represents the weight of the original coordinates
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @param vfAdj Pre-computed vertex-face adjacencies
 */
template<class Mesh>
void meshCotangentSmoothing(
        Mesh& mesh,
        const unsigned int iterations,
        const double alpha,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;
    typedef typename Point::Scalar Scalar;

    std::vector<Point> pointVector(mesh.nextVertexId());

    std::vector<std::vector<VertexId>> vertexRings;

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        pointVector[vId] = mesh.vertexPoint(vId);
    }

    for (unsigned int it = 0; it < iterations; ++it) {
        std::vector<Point> tmpVector = pointVector;

        #pragma omp parallel for
        for (VertexId vId = 0; vId < tmpVector.size(); ++vId) {
            if (mesh.isVertexDeleted(vId))
                continue;

            Scalar weightSum = 0.0;
            if (!vvAdj[vId].empty()) {
                Point value = Point::Zero();

                for (const VertexId& adjVId : vvAdj[vId]) {
                    assert(adjVId != NULL_ID);

                    std::vector<FaceId> adjFaces1;
                    for (const FaceId& adjFId : vfAdj[vId]) {
                        adjFaces1.push_back(adjFId);
                    }
                    std::vector<FaceId> adjFaces2;
                    for (const FaceId& adjFId : vfAdj[adjVId]) {
                        adjFaces2.push_back(adjFId);
                    }
                    std::vector<FaceId> adjFaces;
                    std::set_intersection(adjFaces1.begin(), adjFaces1.end(), adjFaces2.begin(), adjFaces2.end(), std::back_inserter(adjFaces));

                    Scalar adjWeight;
                    if (adjFaces.size() == 2 && adjFaces[0] != adjFaces[1]) {
                        VertexId prevVId = NULL_ID;
                        VertexId nextVId = NULL_ID;

                        const Face& face1 = mesh.face(adjFaces[0]);
                        for (Index pos = 0; pos < face1.vertexNumber(); ++pos) {
                            if (face1.vertexId(pos) != vId && face1.vertexId(pos) != adjVId) {
                                prevVId = face1.vertexId(pos);
                                break;
                            }
                        }

                        const Face& face2 = mesh.face(adjFaces[1]);
                        for (Index pos = 0; pos < face2.vertexNumber(); ++pos) {
                            if (face2.vertexId(pos) != vId && face2.vertexId(pos) != adjVId) {
                                nextVId = face2.vertexId(pos);
                                break;
                            }
                        }

                        assert(prevVId != NULL_ID);
                        assert(nextVId != NULL_ID);

                        adjWeight = cotangentWeight(
                                mesh.vertexPoint(vId),
                                mesh.vertexPoint(adjVId),
                                mesh.vertexPoint(prevVId),
                                mesh.vertexPoint(nextVId));
                    }
                    else {
                        adjWeight = static_cast<Scalar>(1.0);
                    }

                    value += adjWeight * tmpVector[adjVId];
                    weightSum += adjWeight;
                }

                value /= weightSum;

                pointVector[vId] = (alpha * tmpVector[vId]) + ((1 - alpha) * value);
            }
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
 * @brief Cotangent weight laplacian smoothing
 * @param mesh Mesh
 * @param iterations Number of iterations
 * @param alphas Alpha for each vertex. A value between 0 and 1, it represents
 * the weight of the original coordinates
 */
template<class Mesh>
void meshCotangentSmoothing(
        Mesh& mesh,
        const unsigned int iterations,
        const std::vector<double>& alphas)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    const std::vector<std::vector<FaceId>> vfAdj = meshVertexFaceAdjacencies(mesh);
    meshCotangentSmoothing(mesh, iterations, alphas, vvAdj, vfAdj);
}

/**
 * @brief Cotangent weight laplacian smoothing
 * @param mesh Mesh
 * @param iterations Number of iterations
 * @param alphas Alpha for each vertex. A value between 0 and 1, it represents
 * the weight of the original coordinates
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @param vfAdj Pre-computed vertex-face adjacencies
 */
template<class Mesh>
void meshCotangentSmoothing(
        Mesh& mesh,
        const unsigned int iterations,
        const std::vector<double>& alphas,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;
    typedef typename Point::Scalar Scalar;

    std::vector<Point> pointVector(mesh.nextVertexId());

    std::vector<std::vector<VertexId>> vertexRings;

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        pointVector[vId] = mesh.vertexPoint(vId);
    }

    for (unsigned int it = 0; it < iterations; ++it) {
        std::vector<Point> tmpVector = pointVector;

        #pragma omp parallel for
        for (VertexId vId = 0; vId < tmpVector.size(); ++vId) {
            if (mesh.isVertexDeleted(vId))
                continue;

            Scalar weightSum = 0.0;
            if (!vvAdj[vId].empty()) {
                Point value = Point::Zero();

                for (const VertexId& adjVId : vvAdj[vId]) {
                    assert(adjVId != NULL_ID);

                    std::vector<FaceId> adjFaces1;
                    for (const FaceId& adjFId : vfAdj[vId]) {
                        adjFaces1.push_back(adjFId);
                    }
                    std::vector<FaceId> adjFaces2;
                    for (const FaceId& adjFId : vfAdj[adjVId]) {
                        adjFaces2.push_back(adjFId);
                    }
                    std::vector<FaceId> adjFaces;
                    std::set_intersection(adjFaces1.begin(), adjFaces1.end(), adjFaces2.begin(), adjFaces2.end(), std::back_inserter(adjFaces));

                    Scalar adjWeight;
                    if (adjFaces.size() == 2 && adjFaces[0] != adjFaces[1]) {
                        VertexId prevVId;
                        VertexId nextVId;

                        const Face& face1 = mesh.face(adjFaces[0]);
                        for (Index pos = 0; pos < face1.vertexNumber(); ++pos) {
                            if (face1.vertexId(pos) != vId && face1.vertexId(pos) != adjVId) {
                                prevVId = face1.vertexId(pos);
                                break;
                            }
                        }

                        const Face& face2 = mesh.face(adjFaces[1]);
                        for (Index pos = 0; pos < face2.vertexNumber(); ++pos) {
                            if (face2.vertexId(pos) != vId && face2.vertexId(pos) != adjVId) {
                                prevVId = face2.vertexId(pos);
                                break;
                            }
                        }

                        adjWeight = cotangentWeight(
                                mesh.vertexPoint(vId),
                                mesh.vertexPoint(adjVId),
                                mesh.vertexPoint(prevVId),
                                mesh.vertexPoint(nextVId));
                    }
                    else {
                        adjWeight = static_cast<Scalar>(1.0);
                    }

                    value += adjWeight * tmpVector[adjVId];
                    weightSum += adjWeight;
                }

                value /= weightSum;

                pointVector[vId] = (alphas[vId] * tmpVector[vId]) + ((1 - alphas[vId]) * value);
            }
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
 * @brief Cotangent weight laplacian smoothing
 * @param mesh Mesh
 * @param vertices Vertices to smooth
 * @param iterations Number of iterations
 * @param alpha Constant alpha for each vertex. A value between 0 and 1, it
 * represents the weight of the original coordinates
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @param vfAdj Pre-computed vertex-face adjacencies
 */
template<class Mesh>
void meshCotangentSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const double alpha)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    const std::vector<std::vector<FaceId>> vfAdj = meshVertexFaceAdjacencies(mesh);
    meshCotangentSmoothing(mesh, vertices, iterations, alpha, vvAdj, vfAdj);
}


/**
 * @brief Cotangent weight laplacian smoothing
 * @param mesh Mesh
 * @param vertices Vertices to smooth
 * @param iterations Number of iterations
 * @param alpha Constant alpha for each vertex. A value between 0 and 1, it
 * represents the weight of the original coordinates
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @param vfAdj Pre-computed vertex-face adjacencies
 */
template<class Mesh>
void meshCotangentSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const double alpha,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;
    typedef typename Point::Scalar Scalar;

    std::vector<Point> pointVector(mesh.nextVertexId());

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        pointVector[vId] = mesh.vertexPoint(vId);
    }

    for (unsigned int it = 0; it < iterations; ++it) {
        std::vector<Point> tmpVector = pointVector;

        #pragma omp parallel for
        for (Index i = 0; i < vertices.size(); i++) {
            const VertexId& vId = vertices[i];

            Scalar weightSum = 0.0;
            if (!vvAdj[vId].empty()) {
                Point value = Point::Zero();

                for (const VertexId& adjVId : vvAdj[vId]) {
                    assert(adjVId != NULL_ID);

                    std::vector<FaceId> adjFaces1;
                    for (const FaceId& adjFId : vfAdj[vId]) {
                        adjFaces1.push_back(adjFId);
                    }
                    std::vector<FaceId> adjFaces2;
                    for (const FaceId& adjFId : vfAdj[adjVId]) {
                        adjFaces2.push_back(adjFId);
                    }
                    std::vector<FaceId> adjFaces;
                    std::set_intersection(adjFaces1.begin(), adjFaces1.end(), adjFaces2.begin(), adjFaces2.end(), std::back_inserter(adjFaces));

                    Scalar adjWeight;
                    if (adjFaces.size() == 2 && adjFaces[0] != adjFaces[1]) {
                        VertexId prevVId;
                        VertexId nextVId;

                        const Face& face1 = mesh.face(adjFaces[0]);
                        for (Index pos = 0; pos < face1.vertexNumber(); ++pos) {
                            if (face1.vertexId(pos) != vId && face1.vertexId(pos) != adjVId) {
                                prevVId = face1.vertexId(pos);
                                break;
                            }
                        }

                        const Face& face2 = mesh.face(adjFaces[1]);
                        for (Index pos = 0; pos < face2.vertexNumber(); ++pos) {
                            if (face2.vertexId(pos) != vId && face2.vertexId(pos) != adjVId) {
                                prevVId = face2.vertexId(pos);
                                break;
                            }
                        }

                        adjWeight = cotangentWeight(
                                mesh.vertexPoint(vId),
                                mesh.vertexPoint(adjVId),
                                mesh.vertexPoint(prevVId),
                                mesh.vertexPoint(nextVId));
                    }
                    else {
                        adjWeight = static_cast<Scalar>(1.0);
                    }

                    value += adjWeight * tmpVector[adjVId];
                    weightSum += adjWeight;
                }

                value /= weightSum;

                pointVector[vId] = (alpha * tmpVector[vId]) + ((1 - alpha) * value);
            }
        }
    }

    #pragma omp parallel for
    for (VertexId i = 0; i < vertices.size(); i++) {
        const VertexId& vId = vertices[i];

        mesh.setVertexPoint(vId, pointVector[vId]);
    }
}

/**
 * @brief Cotangent weight laplacian smoothing
 * @param mesh Mesh
 * @param vertices Vertices to smooth
 * @param iterations Number of iterations
 * @param alphas Alpha for each vertex. A value between 0 and 1, it represents
 * the weight of the original coordinates
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @param vfAdj Pre-computed vertex-face adjacencies
 */
template<class Mesh>
void meshCotangentSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const std::vector<double>& alphas)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    const std::vector<std::vector<VertexId>> vvAdj = meshVertexVertexAdjacencies(mesh);
    const std::vector<std::vector<FaceId>> vfAdj = meshVertexFaceAdjacencies(mesh);
    meshCotangentSmoothing(mesh, vertices, iterations, alphas, vvAdj, vfAdj);
}


/**
 * @brief Cotangent weight laplacian smoothing
 * @param mesh Mesh
 * @param vertices Vertices to smooth
 * @param iterations Number of iterations
 * @param alphas Alpha for each vertex. A value between 0 and 1, it represents
 * the weight of the original coordinates
 * @param vvAdj Pre-computed vertex-vertex adjacencies
 * @param vfAdj Pre-computed vertex-face adjacencies
 */
template<class Mesh>
void meshCotangentSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const unsigned int iterations,
        const std::vector<double>& alphas,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::Face Face;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;
    typedef typename Point::Scalar Scalar;

    std::vector<Point> pointVector(mesh.nextVertexId());

    #pragma omp parallel for
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        pointVector[vId] = mesh.vertexPoint(vId);
    }

    for (unsigned int it = 0; it < iterations; ++it) {
        std::vector<Point> tmpVector = pointVector;

        #pragma omp parallel for
        for (Index i = 0; i < vertices.size(); i++) {
            const VertexId& vId = vertices[i];

            Scalar weightSum = 0.0;
            if (!vvAdj[vId].empty()) {
                Point value = Point::Zero();

                for (const VertexId& adjVId : vvAdj[vId]) {
                    assert(adjVId != NULL_ID);

                    std::vector<FaceId> adjFaces1;
                    for (const FaceId& adjFId : vfAdj[vId]) {
                        adjFaces1.push_back(adjFId);
                    }
                    std::vector<FaceId> adjFaces2;
                    for (const FaceId& adjFId : vfAdj[adjVId]) {
                        adjFaces2.push_back(adjFId);
                    }
                    std::vector<FaceId> adjFaces;
                    std::set_intersection(adjFaces1.begin(), adjFaces1.end(), adjFaces2.begin(), adjFaces2.end(), std::back_inserter(adjFaces));

                    Scalar adjWeight;
                    if (adjFaces.size() == 2 && adjFaces[0] != adjFaces[1]) {
                        VertexId prevVId;
                        VertexId nextVId;

                        const Face& face1 = mesh.face(adjFaces[0]);
                        for (Index pos = 0; pos < face1.vertexNumber(); ++pos) {
                            if (face1.vertexId(pos) != vId && face1.vertexId(pos) != adjVId) {
                                prevVId = face1.vertexId(pos);
                                break;
                            }
                        }

                        const Face& face2 = mesh.face(adjFaces[1]);
                        for (Index pos = 0; pos < face2.vertexNumber(); ++pos) {
                            if (face2.vertexId(pos) != vId && face2.vertexId(pos) != adjVId) {
                                prevVId = face2.vertexId(pos);
                                break;
                            }
                        }

                        adjWeight = cotangentWeight(
                                mesh.vertexPoint(vId),
                                mesh.vertexPoint(adjVId),
                                mesh.vertexPoint(prevVId),
                                mesh.vertexPoint(nextVId));
                    }
                    else {
                        adjWeight = static_cast<Scalar>(1.0);
                    }

                    value += adjWeight * tmpVector[adjVId];
                    weightSum += adjWeight;
                }

                value /= weightSum;

                pointVector[vId] = (alphas[vId] * tmpVector[vId]) + ((1 - alphas[vId]) * value);
            }
        }
    }

    #pragma omp parallel for
    for (VertexId i = 0; i < vertices.size(); i++) {
        const VertexId& vId = vertices[i];

        mesh.setVertexPoint(vId, pointVector[vId]);
    }
}

}
