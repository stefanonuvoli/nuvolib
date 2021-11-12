#ifndef NVL_LIBIGL_IGL_GEODESICS
#define NVL_LIBIGL_IGL_GEODESICS

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

#include "igl_geodesics.cpp"

#endif // NVL_LIBIGL_IGL_GEODESICS
