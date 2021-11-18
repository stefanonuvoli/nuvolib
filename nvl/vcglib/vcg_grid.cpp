#include "vcg_grid.h"

#include <nvl/models/algorithms/mesh_triangulation.h>

#include <nvl/vcglib/vcg_convert.h>

#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/closest.h>

namespace nvl {

template<class Mesh>
VCGGrid<Mesh>::VCGGrid()
{

}

template<class Mesh>
VCGGrid<Mesh>::VCGGrid(const Mesh& mesh)
{
    this->setMesh(mesh);
}


template<class Mesh>
void VCGGrid<Mesh>::setMesh(const Mesh& mesh)
{
    this->vMesh = mesh;

    this->vTriangulatedMesh = mesh;
    meshTriangulateConvexFace(vTriangulatedMesh, vTriangulationBirthFace);

    convertMeshToVCGMesh(vTriangulatedMesh, vVCGTriangleMesh, vVCGBirthVertex, vVCGBirthFace);

    vcg::tri::UpdateNormal<VCGTriangleMesh>::PerFaceNormalized(vVCGTriangleMesh);
    vcg::tri::UpdateNormal<VCGTriangleMesh>::PerVertexNormalized(vVCGTriangleMesh);
    vcg::tri::UpdateBounding<VCGTriangleMesh>::Box(vVCGTriangleMesh);
    bbMaxDiag = vVCGTriangleMesh.bbox.Diag();
    bbMinDiag = 0;

    grid.Set(vVCGTriangleMesh.face.begin(), vVCGTriangleMesh.face.end());
}

template<class Mesh>
const Mesh& VCGGrid<Mesh>::VCGGrid::triangulatedMesh() const
{
    return vTriangulatedMesh;
}

template<class Mesh>
const Mesh& VCGGrid<Mesh>:: VCGGrid::mesh() const
{
    return vMesh;
}

template<class Mesh>
typename Mesh::FaceId VCGGrid<Mesh>::getClosestFace(const Point& point, Point& closestPoint)
{
    FaceId dummyFaceId;
    Point dummyPoint;
    return getClosestFace(point, closestPoint, dummyFaceId, dummyPoint);
}

template<class Mesh>
typename Mesh::FaceId VCGGrid<Mesh>::getClosestFace(const Point& point, Point& closestPoint, FaceId& triangleFaceId, Point& barycentricCoordinates)
{
    typename VCGTriangleMesh::CoordType vcgClosestPoint;
    typename VCGTriangleMesh::CoordType vcgPoint(point.x(), point.y(), point.z());
    typename VCGTriangleMesh::CoordType vcgNormal;
    typename VCGTriangleMesh::CoordType vcgBarycentricCoordinates;

    typename VCGTriangleMesh::FaceType* facePointer =
            vcg::tri::GetClosestFaceBase<VCGTriangleMesh>(
                vVCGTriangleMesh,
                grid,
                vcgPoint,
                bbMaxDiag,
                bbMinDiag,
                vcgClosestPoint,
                vcgNormal,
                vcgBarycentricCoordinates);

    size_t vcgId = vcg::tri::Index(vVCGTriangleMesh, facePointer);
    closestPoint = Point(vcgClosestPoint.X(), vcgClosestPoint.Y(), vcgClosestPoint.Z());

    triangleFaceId = vVCGBirthFace[vcgId];
    barycentricCoordinates = Point(vcgBarycentricCoordinates.X(), vcgBarycentricCoordinates.Y(), vcgBarycentricCoordinates.Z());

    return vTriangulationBirthFace[triangleFaceId];
}

template<class Mesh>
typename Mesh::VertexId VCGGrid<Mesh>::getClosestVertex(const Point& point)
{
    typename VCGTriangleMesh::CoordType vcgClosestPoint;
    typename VCGTriangleMesh::CoordType vcgPoint(point.x(), point.y(), point.z());

    typename VCGTriangleMesh::VertexType* vertexPointer =
            vcg::tri::GetClosestVertex<VCGTriangleMesh>(
                vVCGTriangleMesh,
                grid,
                vcgPoint,
                bbMaxDiag,
                bbMinDiag);

    size_t vcgId = vcg::tri::Index(vVCGTriangleMesh, vertexPointer);

    return vVCGBirthVertex[vcgId];
}

}
