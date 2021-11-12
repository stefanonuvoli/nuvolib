#include "vcg_collapse_borders.h"

#include <nvl/math/inverse_map.h>

#include <nvl/vcglib/vcg_convert.h>
#include <nvl/vcglib/vcg_triangle_mesh.h>

#include <vcg/complex/algorithms/update/topology.h>
#include <vcg/complex/algorithms/update/flag.h>
#include <vcg/complex/algorithms/edge_collapse.h>
#include <vcg/complex/allocate.h>

namespace nvl {

template<class Mesh>
std::vector<typename Mesh::VertexId> collapseBorders(
    Mesh& mesh,
    const std::vector<typename Mesh::VertexId>& verticesToKeep)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename nvl::Index Index;

    nvl::VCGTriangleMesh vcgMesh;
    std::vector<VertexId> meshBirthVertex;
    std::vector<FaceId> meshBirthFace;
    nvl::convertMeshToVCGMesh(mesh, vcgMesh, meshBirthVertex, meshBirthFace);

    std::vector<Index> vcgVertexMap = nvl::inverseMap(meshBirthVertex, mesh.nextVertexId());

    std::vector<typename Mesh::VertexId> vcgVerticesToKeep(verticesToKeep.size());

    #pragma omp parallel for
    for (Index i = 0; i < verticesToKeep.size(); ++i) {
        const VertexId& vId = verticesToKeep[i];
        assert(vcgVertexMap[vId] != nvl::MAX_INDEX);
        vcgVerticesToKeep[i] = vcgVertexMap[vId];
    }

    std::vector<size_t> vcgNonCollapsed = vcgCollapseBorders(vcgMesh, vcgVerticesToKeep);

    mesh.clear();
    std::vector<VertexId> vcgBirthVertex;
    std::vector<FaceId> vcgBirthFace;
    nvl::convertVCGMeshToMesh(vcgMesh, mesh, vcgBirthVertex, vcgBirthFace);

    std::vector<Index> meshVertexMap = nvl::inverseMap(vcgBirthVertex, vcgMesh.vert.size());
    
    std::vector<VertexId> nonCollapsed(vcgNonCollapsed.size());
    for (Index i = 0; i < vcgNonCollapsed.size(); ++i) {
        assert(meshVertexMap[vcgNonCollapsed[i]] != nvl::MAX_INDEX);
        nonCollapsed[i] = meshVertexMap[vcgNonCollapsed[i]];
    }

    return nonCollapsed;
}


template<class VCGMesh>
std::vector<size_t> vcgCollapseBorders(
    VCGMesh& vcgMesh,
    const std::vector<size_t>& verticesToKeep)
{
    typedef typename nvl::VCGTriangleMesh::VertexType VCGVertexType;
    typedef typename nvl::VCGTriangleMesh::CoordType VCGCoordType;
    typedef typename vcg::tri::BasicVertexPair<VCGVertexType> VertexPair;

    vcg::tri::UpdateTopology<nvl::VCGTriangleMesh>::FaceFace(vcgMesh);
    vcg::tri::UpdateTopology<nvl::VCGTriangleMesh>::VertexFace(vcgMesh);
    vcg::tri::UpdateFlags<nvl::VCGTriangleMesh>::VertexBorderFromFaceAdj(vcgMesh);

    vcg::tri::UpdateFlags<nvl::VCGTriangleMesh>::VertexClearS(vcgMesh);

    #pragma omp parallel for
    for (Index i = 0; i < verticesToKeep.size(); ++i) {
        vcgMesh.vert[verticesToKeep[i]].SetS();
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
                if (vcg::tri::EdgeCollapser<nvl::VCGTriangleMesh,VertexPair>::LinkConditions(VPair)) {
                    vcg::tri::EdgeCollapser<nvl::VCGTriangleMesh,VertexPair>::Do(vcgMesh, VPair, pos);
                    collapsed = true;

                    break;
                }
            }
        }

        if (collapsed) {
            vcg::tri::Allocator<nvl::VCGTriangleMesh>::CompactEveryVector(vcgMesh);
            vcg::tri::UpdateTopology<nvl::VCGTriangleMesh>::FaceFace(vcgMesh);
            vcg::tri::UpdateTopology<nvl::VCGTriangleMesh>::VertexFace(vcgMesh);
            vcg::tri::UpdateFlags<nvl::VCGTriangleMesh>::VertexBorderFromFaceAdj(vcgMesh);
        }
    }
    while (collapsed);

    std::vector<size_t> nonCollapsed;
    for (size_t i = 0; i < vcgMesh.vert.size(); i++) {
        if (vcgMesh.vert[i].IsB() && !vcgMesh.vert[i].IsS()) {
            nonCollapsed.push_back(i);
        }
    }
    return nonCollapsed;
}

}
