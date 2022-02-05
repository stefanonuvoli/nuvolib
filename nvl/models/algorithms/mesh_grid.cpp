/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_grid.h"

#ifdef NVL_VCGLIB_LOADED

#include <nvl/models/algorithms/mesh_triangulation.h>

#include <nvl/models/algorithms/mesh_vcg_convert.h>

#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/closest.h>

namespace nvl {

/**
 * @brief Default constructor
 */
template<class Mesh>
VCGGrid<Mesh>::VCGGrid()
{

}

/**
 * @brief Constructor with mesh
 * @param mesh Mesh
 */
template<class Mesh>
VCGGrid<Mesh>::VCGGrid(const Mesh& mesh)
{
    this->setMesh(mesh);
}

/**
 * @brief Setter of the mesh
 * @param mesh Mesh
 */
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

/**
 * @brief Getter of the triangulated mesh
 * @return Mesh
 */
template<class Mesh>
const Mesh& VCGGrid<Mesh>::VCGGrid::triangulatedMesh() const
{
    return vTriangulatedMesh;
}

/**
 * @brief Getter of the mesh
 * @return Mesh
 */
template<class Mesh>
const Mesh& VCGGrid<Mesh>:: VCGGrid::mesh() const
{
    return vMesh;
}

/**
 * @brief Get closest face
 * @param point Query point
 * @param closestPoint Closest point lying in the face
 * @return Id of the closest face
 */
template<class Mesh>
typename Mesh::FaceId VCGGrid<Mesh>::getClosestFace(const Point& point, Point& closestPoint)
{
    FaceId dummyFaceId;
    Point dummyPoint;
    return getClosestFace(point, closestPoint, dummyFaceId, dummyPoint);
}

/**
 * @brief Get closest face
 * @param point Query point
 * @param closestPoint Closest point lying in the face
 * @param triangleFaceId Id of the closest face in the triangulated mesh
 * @param bayrcentricCoordinates Barycentric coordinates of the point in the
 * closest face of the triangulated mesh
 * @return Id of the closest face
 */
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

/**
 * @brief Get closest verrtex
 * @param point Query point
 * @return Id of the closest vertex
 */
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

#endif

}
