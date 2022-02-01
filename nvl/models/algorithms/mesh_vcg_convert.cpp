/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_vcg_convert.h"

#ifdef NVL_VCGLIB_LOADED

#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/clean.h>

#include <nvl/math/point.h>

namespace nvl {

template<class VCGMesh, class Segment>
void convertSegmentsToVCGEdgeMesh(
        const std::vector<Segment>& segments,
        VCGMesh& vcgMesh)
{
    std::map<typename VCGMesh::CoordType, size_t> vertexMap;
    for (const Segment& segment : segments) {
        typename VCGMesh::CoordType coord1(segment.p1().x(), segment.p1().y(), segment.p1().z());
        if (vertexMap.find(coord1) == vertexMap.end()) {
            vertexMap.insert(std::make_pair(coord1, vcgMesh.vert.size()));
            vcg::tri::Allocator<VCGMesh>::AddVertex(vcgMesh, coord1);
        }

        typename VCGMesh::CoordType coord2(segment.p2().x(), segment.p2().y(), segment.p2().z());
        if (vertexMap.find(coord2) == vertexMap.end()) {
            vertexMap.insert(std::make_pair(coord2, vcgMesh.vert.size()));
            vcg::tri::Allocator<VCGMesh>::AddVertex(vcgMesh, coord2);
        }
    }

    for (const Segment& segment : segments) {
        typename VCGMesh::CoordType coord1(segment.p1().x(), segment.p1().y(), segment.p1().z());
        typename VCGMesh::CoordType coord2(segment.p2().x(), segment.p2().y(), segment.p2().z());
        vcg::tri::Allocator<VCGMesh>::AddEdge(vcgMesh, vertexMap.at(coord1), vertexMap.at(coord2));
    }
}

#ifdef NVL_EIGEN_LOADED

template<class VCGMesh>
void convertVCGMeshToEigen(
        VCGMesh& vcgMesh,
        Eigen::MatrixXd& V,
        Eigen::MatrixXi& F,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace,
        bool selectedOnly,
        Index numVerticesPerFace,
        Index dim)
{
    assert(dim >= 2);
    assert(numVerticesPerFace > 2);

    int nSelectedVertices = 0;
    for (size_t i = 0; i < vcgMesh.vert.size(); ++i){
        if ((!selectedOnly || vcgMesh.vert[i].IsS()) && !vcgMesh.vert[i].IsD()) {
            ++nSelectedVertices;
        }
    }
    int nSelectedFaces = 0;
    for (size_t i = 0; i < vcgMesh.face.size(); ++i){
        if ((!selectedOnly || vcgMesh.face[i].IsS()) && !vcgMesh.face[i].IsD()) {
            ++nSelectedFaces;
        }
    }

    V.resize(nSelectedVertices, dim);
    F.resize(nSelectedFaces, numVerticesPerFace);

    std::vector<Index> vMap(vcgMesh.vert.size(), NULL_ID);
    birthVertex.resize(nSelectedVertices, NULL_ID);
    size_t vId = 0;
    for (size_t i = 0; i < vcgMesh.vert.size(); ++i){
        if ((!selectedOnly || vcgMesh.vert[i].IsS()) && !vcgMesh.vert[i].IsD()) {
            for (Index j = 0; j < dim; ++j) {
                V(vId, j) = vcgMesh.vert[i].P()[j];
            }

            birthVertex[vId] = i;
            vMap[i] = vId;

            ++vId;
        }
    }

    birthFace.resize(nSelectedFaces, NULL_ID);
    size_t fId = 0;
    for (size_t i = 0; i < vcgMesh.face.size(); ++i){
        if ((!selectedOnly || vcgMesh.face[i].IsS()) && !vcgMesh.face[i].IsD()) {
            for (int j = 0; j < vcgMesh.face[i].VN(); ++j) {
                F(fId, j) = vMap[vcg::tri::Index(vcgMesh, vcgMesh.face[i].V(j))];
            }

            birthFace[fId] = i;

            ++fId;
        }
    }
}

template<class VCGMesh>
void convertEigenToVCGMesh(
        const Eigen::MatrixXd& V,
        const Eigen::MatrixXi& F,
        VCGMesh& vcgMesh)
{
    vcgMesh.Clear();

    vcg::tri::Allocator<VCGMesh>::AddVertices(vcgMesh, V.rows());
    for (int i = 0; i < V.rows(); ++i) {
        typename VCGMesh::CoordType vv(V(i,0), V(i,1), V(i,2));
        vcgMesh.vert[static_cast<size_t>(i)].P() = vv;
    }

    vcg::tri::Allocator<VCGMesh>::AddFaces(vcgMesh, static_cast<size_t>(F.rows()));
    for (int i = 0; i < F.rows(); ++i) {
        const Eigen::VectorXi& row = F.row(i);
        Eigen::Index numVertices = row.size();
        vcgMesh.face[static_cast<size_t>(i)].Alloc(numVertices);
        for (Eigen::Index j = 0; j < numVertices; ++j) {
            vcgMesh.face[static_cast<size_t>(i)].V(j) = &(vcgMesh.vert[static_cast<size_t>(F(i,j))]);
        }
    }
}

#endif

template<class Mesh, class VCGMesh>
void convertMeshToVCGMesh(
        const Mesh& mesh,
        VCGMesh& vcgMesh,
        Index dim)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    vcgMesh.Clear();

    Index vcgCurrentVId = 0;
    vcg::tri::Allocator<VCGMesh>::AddVertices(vcgMesh, mesh.vertexNumber());
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        const typename Mesh::Vertex& vertex = mesh.vertex(vId);
        const typename Mesh::Point& point = vertex.point();

        typename VCGMesh::CoordType vcgCoord;
        for (Index j = 0; j < dim; ++j) {
            vcgCoord[j] = point(j);
        }
        vcgMesh.vert[vcgCurrentVId].P() = vcgCoord;

        ++vcgCurrentVId;
    }

    Index vcgCurrentFId = 0;
    vcg::tri::Allocator<VCGMesh>::AddFaces(vcgMesh, mesh.faceNumber());
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const typename Mesh::Face& face = mesh.face(fId);

        vcgMesh.face[vcgCurrentFId].Alloc(face.vertexNumber());
        for (Index j = 0; j < face.vertexNumber(); ++j) {
            vcgMesh.face[vcgCurrentFId].V(j) = &(vcgMesh.vert[face.vertexId(j)]);
        }

        ++vcgCurrentFId;
    }
}

template<class Mesh, class VCGMesh>
void convertMeshToVCGMesh(
        const Mesh& mesh,
        VCGMesh& vcgMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace,
        Index dim)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    vcgMesh.Clear();

    birthVertex.clear();
    birthFace.clear();

    Index vcgCurrentVId = 0;
    vcg::tri::Allocator<VCGMesh>::AddVertices(vcgMesh, mesh.vertexNumber());
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        const typename Mesh::Vertex& vertex = mesh.vertex(vId);
        const typename Mesh::Point& point = vertex.point();

        typename VCGMesh::CoordType vcgCoord;
        for (Index j = 0; j < dim; ++j) {
            vcgCoord[j] = point(j);
        }
        vcgMesh.vert[vcgCurrentVId].P() = vcgCoord;

        assert(birthVertex.size() == vcgCurrentVId);
        birthVertex.push_back(vId);

        ++vcgCurrentVId;
    }

    Index vcgCurrentFId = 0;
    vcg::tri::Allocator<VCGMesh>::AddFaces(vcgMesh, mesh.faceNumber());
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const typename Mesh::Face& face = mesh.face(fId);

        vcgMesh.face[vcgCurrentFId].Alloc(face.vertexNumber());
        for (Index j = 0; j < face.vertexNumber(); ++j) {
            vcgMesh.face[vcgCurrentFId].V(j) = &(vcgMesh.vert[face.vertexId(j)]);
        }

        assert(birthFace.size() == vcgCurrentFId);
        birthFace.push_back(fId);

        ++vcgCurrentFId;
    }
}

template<class Mesh, class VCGMesh>
void convertVCGMeshToMesh(
        VCGMesh& vcgMesh,
        Mesh& mesh,
        bool selectedOnly,
        Index dim)
{
    typedef typename VCGMesh::CoordType VCGCoordType;

    mesh.clear();

    for (Index vId = 0; vId < vcgMesh.vert.size(); ++vId){
        if ((!selectedOnly || vcgMesh.vert[vId].IsS()) && !vcgMesh.vert[vId].IsD()) {
            VCGCoordType coord = vcgMesh.vert[vId].P();

            Point3d p;
            for (Index j = 0; j < dim; ++j) {
                p(j) = coord[j];
            }

            mesh.addVertex(p);
        }
    }

    for (Index fId = 0; fId < vcgMesh.face.size(); ++fId){
        if ((!selectedOnly || vcgMesh.face[fId].IsS()) && !vcgMesh.face[fId].IsD()) {
            std::vector<Index> faceVertices(vcgMesh.face[fId].VN());
            for (Index j = 0; j < static_cast<Index>(vcgMesh.face[fId].VN()); ++j) {
                faceVertices[j] = vcg::tri::Index(vcgMesh, vcgMesh.face[fId].V(j));
            }

            mesh.addFace(faceVertices);
        }
    }
}

template<class Mesh, class VCGMesh>
void convertVCGMeshToMesh(
        VCGMesh& vcgMesh,
        Mesh& mesh,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace,
        bool selectedOnly,
        Index dim)
{
    typedef typename VCGMesh::CoordType VCGCoordType;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    mesh.clear();

    birthVertex.clear();
    birthFace.clear();

    for (Index vId = 0; vId < vcgMesh.vert.size(); ++vId){
        if ((!selectedOnly || vcgMesh.vert[vId].IsS()) && !vcgMesh.vert[vId].IsD()) {
            VCGCoordType coord = vcgMesh.vert[vId].P();

            Point3d p;
            for (Index j = 0; j < dim; ++j) {
                p(j) = coord[j];
            }

            VertexId newId = mesh.addVertex(p);
            NVL_SUPPRESS_UNUSEDVARIABLE(newId);
            assert(birthVertex.size() == newId);
            birthVertex.push_back(vId);
        }
    }

    for (Index fId = 0; fId < vcgMesh.face.size(); ++fId){
        if ((!selectedOnly || vcgMesh.face[fId].IsS()) && !vcgMesh.face[fId].IsD()) {
            std::vector<Index> faceVertices(vcgMesh.face[fId].VN());
            for (Index j = 0; j < static_cast<Index>(vcgMesh.face[fId].VN()); ++j) {
                faceVertices[j] = vcg::tri::Index(vcgMesh, vcgMesh.face[fId].V(j));
            }

            FaceId newId = mesh.addFace(faceVertices);
            NVL_SUPPRESS_UNUSEDVARIABLE(newId);
            assert(birthFace.size() == newId);
            birthFace.push_back(fId);
        }
    }
}

#endif

}
