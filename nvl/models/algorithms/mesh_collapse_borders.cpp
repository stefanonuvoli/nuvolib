/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_collapse_borders.h"

#ifdef NVL_VCGLIB_LOADED

#include <nvl/math/inverse_function.h>

#include <nvl/models/structures/vcg_triangle_mesh.h>
#include <nvl/models/algorithms/mesh_vcg_convert.h>

#include <vcg/complex/algorithms/update/topology.h>
#include <vcg/complex/algorithms/update/flag.h>
#include <vcg/complex/algorithms/edge_collapse.h>
#include <vcg/complex/allocate.h>

namespace nvl {

template<class Mesh>
std::vector<typename Mesh::VertexId> collapseBorders(
    Mesh& mesh,
    const std::vector<typename Mesh::VertexId>& verticesToKeep,
    std::vector<typename Mesh::VertexId>& birthVertex,
    std::vector<typename Mesh::FaceId>& birthFace)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;

    VCGTriangleMesh vcgMesh;
    std::vector<VertexId> meshBirthVertex;
    std::vector<FaceId> meshBirthFace;
    convertMeshToVCGMesh(mesh, vcgMesh, meshBirthVertex, meshBirthFace);

    std::vector<Index> meshVertexMap = inverseFunction(meshBirthVertex, mesh.nextVertexId());

    std::vector<typename Mesh::VertexId> vcgVerticesToKeep(verticesToKeep.size());

    #pragma omp parallel for
    for (Index i = 0; i < verticesToKeep.size(); ++i) {
        const VertexId& vId = verticesToKeep[i];
        assert(meshVertexMap[vId] != NULL_ID);
        vcgVerticesToKeep[i] = meshVertexMap[vId];
    }

    std::vector<Index> vcgCollapseBirthVertex;
    std::vector<Index> vcgCollapseBirthFace;
    std::vector<size_t> vcgNonCollapsed = vcgCollapseBorders(vcgMesh, vcgVerticesToKeep, vcgCollapseBirthVertex, vcgCollapseBirthFace);

    mesh.clear();
    std::vector<Index> vcgBirthVertex;
    std::vector<Index> vcgBirthFace;
    convertVCGMeshToMesh(vcgMesh, mesh, vcgBirthVertex, vcgBirthFace);

    std::vector<Index> vcgVertexMap = inverseFunction(vcgBirthVertex, vcgMesh.vert.size());

    std::vector<VertexId> nonCollapsed(vcgNonCollapsed.size());
    for (Index i = 0; i < vcgNonCollapsed.size(); ++i) {
        assert(vcgVertexMap[vcgNonCollapsed[i]] != NULL_ID);
        nonCollapsed[i] = vcgVertexMap[vcgNonCollapsed[i]];
    }

    birthVertex.resize(mesh.nextVertexId(), NULL_ID);
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        assert(meshBirthVertex[vcgCollapseBirthVertex[vcgBirthVertex[vId]]] != NULL_ID);
        birthVertex[vId] = meshBirthVertex[vcgCollapseBirthVertex[vcgBirthVertex[vId]]];
    }

    birthFace.resize(mesh.nextFaceId(), NULL_ID);
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        assert(meshBirthFace[vcgCollapseBirthFace[vcgBirthFace[fId]]] != NULL_ID);
        birthFace[fId] = meshBirthFace[vcgCollapseBirthFace[vcgBirthFace[fId]]];
    }

    return nonCollapsed;
}


template<class VCGMesh>
std::vector<size_t> vcgCollapseBorders(
    VCGMesh& vcgMesh,
    const std::vector<size_t>& verticesToKeep,
    std::vector<Index>& vcgCollapseBirthVertex,
    std::vector<Index>& vcgCollapseBirthFace)
{
    typedef typename VCGTriangleMesh::VertexType VCGVertexType;
    typedef typename VCGTriangleMesh::CoordType VCGCoordType;
    typedef typename vcg::tri::BasicVertexPair<VCGVertexType> VertexPair;

    vcg::tri::UpdateTopology<VCGTriangleMesh>::FaceFace(vcgMesh);
    vcg::tri::UpdateTopology<VCGTriangleMesh>::VertexFace(vcgMesh);
    vcg::tri::UpdateFlags<VCGTriangleMesh>::VertexBorderFromFaceAdj(vcgMesh);

    vcg::tri::UpdateFlags<VCGTriangleMesh>::VertexClearS(vcgMesh);

    #pragma omp parallel for
    for (Index i = 0; i < verticesToKeep.size(); ++i) {
        vcgMesh.vert[verticesToKeep[i]].SetS();
    }

    for (size_t i = 0; i < vcgMesh.vert.size(); i++) {
        if (vcgMesh.vert[i].IsD())
            continue;

        vcgMesh.vert[i].Q() = i;
    }
    for (size_t i = 0; i < vcgMesh.face.size(); i++) {
        if (vcgMesh.face[i].IsD())
            continue;

        vcgMesh.face[i].Q() = i;
    }

    bool collapsed;
    do {
        collapsed = false;

        std::set<std::pair<VCGCoordType, VCGCoordType>> edgesToCollapse;
        for (size_t i = 0; i < vcgMesh.face.size(); i++) {
            if (vcgMesh.face[i].IsD())
                continue;

            for (size_t j = 0; j < 3; j++) {
                if (!vcg::face::IsBorder(vcgMesh.face[i], j))
                    continue;

                VCGVertexType* v0 = vcgMesh.face[i].V0(j);
                VCGVertexType* v1 = vcgMesh.face[i].V1(j);

                if (v0->IsS() && v1->IsS())
                    continue;

                VCGCoordType p0 = vcgMesh.face[i].P0(j);
                VCGCoordType p1 = vcgMesh.face[i].P1(j);

                std::pair<VCGCoordType,VCGCoordType> key(std::min(p0,p1), std::max(p0,p1));
                edgesToCollapse.insert(key);
            }
        }

        for (size_t i = 0; i < vcgMesh.face.size(); i++) {
            if (vcgMesh.face[i].IsD())
                continue;

            for (size_t j = 0; j < 3; j++) {
                if (!vcg::face::IsBorder(vcgMesh.face[i], j))
                    continue;

                VCGCoordType p0 = vcgMesh.face[i].P0(j);
                VCGCoordType p1 = vcgMesh.face[i].P1(j);

                std::pair<VCGCoordType,VCGCoordType> key(std::min(p0,p1), std::max(p0,p1));
                if (edgesToCollapse.count(key) == 0)
                    continue;

                VCGVertexType* v0 = vcgMesh.face[i].V0(j);
                VCGVertexType* v1 = vcgMesh.face[i].V1(j);
                assert(!((v0->IsS())&&(v1->IsS())));

                VCGCoordType pos = (p0+p1)/2;
                //v1 always survive
                if (v0->IsS())
                    std::swap(v0, v1);

                //at this point only v1 can be selected
                assert(!v0->IsS());
                if (v1->IsS())
                    pos = v1->P();

                VertexPair VPair(v0, v1);
                if (vcg::tri::EdgeCollapser<VCGTriangleMesh,VertexPair>::LinkConditions(VPair)) {
                    vcg::tri::EdgeCollapser<VCGTriangleMesh,VertexPair>::Do(vcgMesh, VPair, pos);
                    collapsed = true;

                    break;
                }
            }
        }

        if (collapsed) {
            vcg::tri::Allocator<VCGTriangleMesh>::CompactEveryVector(vcgMesh);
            vcg::tri::UpdateTopology<VCGTriangleMesh>::FaceFace(vcgMesh);
            vcg::tri::UpdateTopology<VCGTriangleMesh>::VertexFace(vcgMesh);
            vcg::tri::UpdateFlags<VCGTriangleMesh>::VertexBorderFromFaceAdj(vcgMesh);
        }
    }
    while (collapsed);

    std::vector<size_t> nonCollapsed;
    for (size_t i = 0; i < vcgMesh.vert.size(); i++) {
        assert(!vcgMesh.vert[i].IsD());
        if (vcgMesh.vert[i].IsB() && !vcgMesh.vert[i].IsS()) {
            nonCollapsed.push_back(i);
        }
    }

    vcgCollapseBirthVertex.resize(vcgMesh.vert.size());
    for (size_t i = 0; i < vcgMesh.vert.size(); i++) {
        assert(!vcgMesh.vert[i].IsD());
        vcgCollapseBirthVertex[i] = vcgMesh.vert[i].Q();
    }

    vcgCollapseBirthFace.resize(vcgMesh.face.size());
    for (size_t i = 0; i < vcgMesh.face.size(); i++) {
        assert(!vcgMesh.face[i].IsD());
        vcgCollapseBirthFace[i] = vcgMesh.face[i].Q();
    }

    return nonCollapsed;
}

#endif

}
