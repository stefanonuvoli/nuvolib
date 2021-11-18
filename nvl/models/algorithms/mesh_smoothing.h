#ifndef NVL_MODELS_MESH_SMOOTHING_H
#define NVL_MODELS_MESH_SMOOTHING_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

/* Laplacian smoothing */

template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const Index iterations,
        const double alpha);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const Index iterations,
        const double alpha,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<double>& alphas,
        const Index iterations);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<double>& alphas,
        const Index iterations,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);

template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const Index iterations,
        const double alpha);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const Index iterations,
        const double alpha,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const Index iterations,
        const std::vector<double>& alphas);
template<class Mesh>
void meshLaplacianSmoothing(
        Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        const Index iterations,
        const std::vector<double>& alphas,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);
}

#include "mesh_smoothing.cpp"

#endif // NVL_MODELS_MESH_SMOOTHING_H
