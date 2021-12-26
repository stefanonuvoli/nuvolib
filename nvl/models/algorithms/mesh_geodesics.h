#ifndef NVL_MODELS_MESH_GEODESICS
#define NVL_MODELS_MESH_GEODESICS

#ifdef NVL_LIBIGL_LOADED

#include <nvl/nuvolib.h>

#include <igl/heat_geodesics.h>

#include <vector>
#include <utility>

namespace nvl {

template<class Mesh>
struct HeatGeodesicsData {
    igl::HeatGeodesicsData<typename Mesh::Scalar> data;
    std::vector<typename Mesh::VertexId> birthVertex;
};

template<class Mesh>
std::vector<typename Mesh::Scalar> exactGeodesics(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& sourceVertices,
        const std::vector<typename Mesh::VertexId>& targetVertices);

template<class Mesh>
std::vector<typename Mesh::Scalar> exactGeodesics(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& sourceVertices);

template<class Mesh>
void heatGeodesicsPrecomputeData(
        const Mesh& mesh,
        HeatGeodesicsData<Mesh>& precomputedData);

template<class Mesh>
std::vector<typename Mesh::Scalar> heatGeodesics(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& sourceVertices);

template<class Mesh>
std::vector<typename Mesh::Scalar> heatGeodesics(
        const Mesh& mesh,
        const HeatGeodesicsData<Mesh>& precomputedData,
        const std::vector<typename Mesh::VertexId>& sourceVertices);

}

#endif

#include "mesh_geodesics.cpp"

#endif // NVL_MODELS_MESH_GEODESICS
