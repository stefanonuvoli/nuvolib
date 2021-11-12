#ifndef NVL_VCGLIB_VCG_GRID_H
#define NVL_VCGLIB_VCG_GRID_H

#include <nvl/nuvolib.h>

#include <nvl/vcglib/vcg_triangle_mesh.h>

#include <vector>
#include <utility>

#include <vcg/complex/complex.h>
#include <vcg/space/index/grid_static_ptr.h>

namespace nvl {

template<class Mesh>
class VCGGrid {

public:

    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    VCGGrid();
    VCGGrid(const Mesh& vMesh);

    void setMesh(const Mesh& vMesh);

    const Mesh& triangulatedMesh() const;
    const Mesh& mesh() const;

    VCGGrid::FaceId getClosestFace(const VCGGrid::Point& point, Point& closestPoint, VCGGrid::FaceId& triangleFaceId, Point& barycentricCoordinates);
    VCGGrid::FaceId getClosestFace(const VCGGrid::Point& point, Point& closestPoint);
    VCGGrid::VertexId getClosestVertex(const VCGGrid::Point& point);

private:

    Mesh vMesh;
    Mesh vTriangulatedMesh;

    vcg::GridStaticPtr<typename VCGTriangleMesh::FaceType, typename VCGTriangleMesh::FaceType::ScalarType> grid;
    VCGTriangleMesh vVCGTriangleMesh;

    std::vector<FaceId> vTriangulationBirthFace;
    std::vector<VertexId> vVCGBirthVertex;
    std::vector<FaceId> vVCGBirthFace;

    typename VCGTriangleMesh::FaceType::ScalarType bbMaxDiag;
    typename VCGTriangleMesh::FaceType::ScalarType bbMinDiag;

};

}

#include "vcg_grid.cpp"

#endif // NVL_VCGLIB_VCG_GRID_H
