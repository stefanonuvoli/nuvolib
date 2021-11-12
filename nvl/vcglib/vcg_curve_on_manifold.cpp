#include "vcg_curve_on_manifold.h"

#include <nvl/math/comparisons.h>
#include <nvl/math/numeric_limits.h>

#include <nvl/vcglib/vcg_triangle_mesh.h>
#include <nvl/vcglib/vcg_convert.h>

#include <nvl/models/mesh_triangulation.h>

#include <vcg/complex/algorithms/curve_on_manifold.h>

namespace nvl {

template<class Mesh>
Mesh curveOnManifold(
        const Mesh& mesh,
        const std::vector<Segment<typename Mesh::Point>>& segments,
        std::vector<std::pair<typename Mesh::VertexId, typename Mesh::VertexId>>& curveLines,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace,
        const int& firstStepIterations,
        const int& secondStepIterations,
        const double& baryCentricThreshold,
        const bool fixBorders,
        const bool fixCorners)
{
    typedef nvl::VCGTriangleMesh VCGTriangleMesh;

    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    Mesh outputMesh;

    curveLines.clear();
    birthVertex.clear();
    birthFace.clear();

    if (segments.size() < 3) {
        nvl::meshTransferFaces(mesh, outputMesh, birthVertex, birthFace);
        return outputMesh;
    }

    Mesh triangulatedMesh = mesh;
    std::vector<FaceId> triangulatedBirthFace;
    meshTriangulateConvexFace(triangulatedMesh, triangulatedBirthFace);

    VCGTriangleMesh vcgMesh, vcgResultMesh, edgeMesh;

    std::vector<VertexId> vcgBirthVertex;
    std::vector<FaceId> vcgBirthFace;
    convertMeshToVCGMesh(triangulatedMesh, vcgMesh, vcgBirthVertex, vcgBirthFace);

    convertSegmentsToVCGEdgeMesh(segments, edgeMesh);

    std::vector<Index> comBirthVertex;
    std::vector<Index> comBirthFace;
    vcgCurveOnManifold(vcgMesh, edgeMesh, vcgResultMesh, comBirthVertex, comBirthFace, firstStepIterations, secondStepIterations, baryCentricThreshold, fixBorders, fixCorners);

    for (size_t i = 0; i < vcgResultMesh.face.size(); ++i) {
        if (!vcgResultMesh.face[i].IsD()) {
            for (int k = 0; k < vcgResultMesh.face[i].VN(); ++k) {
                if (vcgResultMesh.face[i].IsFaceEdgeS(k)) {
                    const Index vId1 = vcg::tri::Index(vcgResultMesh, vcgResultMesh.face[i].V0(k));
                    const Index vId2 = vcg::tri::Index(vcgResultMesh, vcgResultMesh.face[i].V1(k));

                    curveLines.push_back(std::make_pair(std::min(vId1,vId2), std::max(vId1,vId2)));
                }
            }
        }
    }

    //Sort and erase duplicates
    std::sort(curveLines.begin(), curveLines.end());
    curveLines.erase(std::unique(curveLines.begin(), curveLines.end()), curveLines.end());

    Mesh triangulatedResultMesh;
    std::vector<Index> triangulatedResultBirthVertex;
    std::vector<Index> triangulatedResultBirthFace;
    convertVCGMeshToMesh(vcgResultMesh, triangulatedResultMesh, triangulatedResultBirthVertex, triangulatedResultBirthFace);

    birthVertex.resize(triangulatedResultMesh.nextVertexId(), MAX_INDEX);
    for (VertexId vId = 0; vId < triangulatedResultMesh.nextVertexId(); ++vId) {
        if (!triangulatedResultMesh.isVertexDeleted(vId)) {
            Index mapping;

            mapping = triangulatedResultBirthVertex[vId];
            assert(mapping != MAX_INDEX);
            mapping = comBirthVertex[mapping];
            if (mapping == MAX_INDEX)
                continue;
            mapping = vcgBirthVertex[mapping];
            assert(mapping != MAX_INDEX);

            birthVertex[vId] = mapping;
        }
    }

    std::vector<FaceId> tmpResultBirthFace(triangulatedResultMesh.nextFaceId(), MAX_INDEX);
    for (FaceId fId = 0; fId < triangulatedResultMesh.nextFaceId(); ++fId) {
        if (!triangulatedResultMesh.isFaceDeleted(fId)) {
            Index mapping;

            mapping = triangulatedResultBirthFace[fId];
            assert(mapping != MAX_INDEX);
            mapping = comBirthFace[mapping];
            assert(mapping != MAX_INDEX);
            mapping = vcgBirthFace[mapping];
            assert(mapping != MAX_INDEX);
            mapping = triangulatedBirthFace[mapping];
            assert(mapping != MAX_INDEX);

            tmpResultBirthFace[fId] = mapping;
        }
    }

    outputMesh = meshReconstructTriangulatedPolygons(mesh, triangulatedResultMesh, birthVertex, tmpResultBirthFace, birthFace);

    return outputMesh;
}

template<class VCGMesh>
bool vcgCurveOnManifold(
        VCGMesh& mesh,
        VCGMesh& edgeMesh,
        VCGMesh& resultMesh,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace,
        const int& firstStepIterations,
        const int& secondStepIterations,
        const double& baryCentricThreshold,
        const bool fixBorders,
        const bool fixCorners)
{
    typedef typename VCGTriangleMesh::CoordType CoordType;
    typedef typename VCGTriangleMesh::ScalarType ScalarType;

    vcg::tri::Append<VCGMesh,VCGMesh>::MeshCopy(resultMesh, mesh);

    vcg::tri::UpdateTopology<VCGMesh>::FaceFace(resultMesh);
    vcg::tri::UpdateBounding<VCGMesh>::Box(resultMesh);
    vcg::tri::UpdateQuality<VCGMesh>::FaceConstant(resultMesh, -1);
    vcg::tri::UpdateQuality<VCGMesh>::VertexConstant(resultMesh, -1);

    #pragma omp parallel for
    for (Index i = 0; i < resultMesh.vert.size(); ++i) {
        if (!resultMesh.vert[i].IsD()) {
            resultMesh.vert[i].Q() = i;
        }
    }
    #pragma omp parallel for
    for (Index i = 0; i < resultMesh.face.size(); ++i) {
        if (!resultMesh.face[i].IsD()) {
            resultMesh.face[i].Q() = i;
        }
    }

    vcg::tri::CoM<VCGMesh> cc(resultMesh);

    cc.Init();

    if (fixBorders) {
        cc.SelectBoundaryVertex(edgeMesh);
    }
    else {
        vcg::tri::UpdateSelection<VCGMesh>::VertexClear(edgeMesh);
        vcg::tri::UpdateTopology<VCGMesh>::VertexEdge(edgeMesh);
    }

    if (fixCorners) {
        vcg::tri::ForEachVertex(edgeMesh, [&](typename VCGMesh::VertexType &v){
          if(vcg::edge::VEDegree<typename VCGMesh::EdgeType>(&v)!=2) v.SetS();
        });
    }

    // Two smoothing runs,
    // the first that allows fast movement over the surface (long edges that can skim surface details)
    cc.par.surfDistThr = resultMesh.bbox.Diag()/100.0f;
    cc.par.maxSimpEdgeLen = resultMesh.bbox.Diag()/50.0f;
    cc.par.minRefEdgeLen = resultMesh.bbox.Diag()/100.0f;
    cc.par.barycentricSnapThr = baryCentricThreshold;
    cc.SmoothProject(edgeMesh,firstStepIterations,0.7f,.3f);

    // The second smooting run more accurate to adapt to the surface
    cc.par.surfDistThr = resultMesh.bbox.Diag()/1000.0f;
    cc.par.maxSimpEdgeLen = resultMesh.bbox.Diag()/1000.0f;
    cc.par.minRefEdgeLen = resultMesh.bbox.Diag()/2000.0f;
    cc.par.barycentricSnapThr = baryCentricThreshold;
    cc.SmoothProject(edgeMesh,secondStepIterations,0.01f,.99f);

    // Adapt the polyline to the mesh (in the end it will have vertices only on edges and vertices of the base mesh)
    cc.RefineCurveByBaseMesh(edgeMesh);

    // Safely split the mesh with this refined polyline
    cc.SplitMeshWithPolyline(edgeMesh);

    // Now the two meshes should have coincident edges
    bool done = cc.TagFaceEdgeSelWithPolyLine(edgeMesh);
    if (!done) {
        std::cout << "Warning: vcglib bug of curve on manifold! Edges have not been tagged properly." << std::endl;


        size_t totalEdgesSelected = 0;
        for (size_t i = 0; i < resultMesh.face.size(); ++i) {
            if (!resultMesh.face[i].IsD()) {
                for (int k = 0; k < resultMesh.face[i].VN(); ++k) {
                    if (resultMesh.face[i].IsFaceEdgeS(k)) {
                        ++totalEdgesSelected;
                    }
                }
            }
        }
        std::cout << "Before fixing curve on manifold bug: " << totalEdgesSelected << " selected. This number should be: " << edgeMesh.EN()*2 << std::endl;

        vcg::tri::UpdateBounding<VCGTriangleMesh>::Box(resultMesh);
        vcg::tri::UpdateTopology<VCGTriangleMesh>::FaceFace(resultMesh);
        ScalarType maxD=resultMesh.bbox.Diag();
        ScalarType minD=0;

        vcg::GridStaticPtr<typename VCGTriangleMesh::FaceType,typename VCGTriangleMesh::FaceType::ScalarType> Grid;
        Grid.Set(resultMesh.face.begin(), resultMesh.face.end());

        size_t newEdgesSelected = 0;
        for (size_t i = 0; i < edgeMesh.edge.size(); ++i) {
            if (!edgeMesh.edge[i].IsD()) {
                const CoordType& p1 = edgeMesh.edge[i].V0(0)->cP();
                const CoordType& p2 = edgeMesh.edge[i].V1(0)->cP();

                CoordType midPoint = (p1 + p2) / 2;

                typename VCGTriangleMesh::CoordType closestPoint;
                typename VCGTriangleMesh::FaceType* f =
                        vcg::tri::GetClosestFaceBase<VCGTriangleMesh>(
                            resultMesh,
                            Grid,
                            midPoint,
                            maxD,minD,
                            closestPoint);

                int bestEdge = 0;
                ScalarType bestDistance = nvl::maxLimitValue<ScalarType>();

                for (int j = 0; j < f->VN(); ++j) {
                    CoordType point = (f->V0(j)->cP() + f->V1(j)->cP()) / 2;

                    ScalarType distance = (point - closestPoint).Norm();

                    if (distance < bestDistance) {
                        bestEdge = j;
                        bestDistance = distance;
                    }
                }

                typename VCGTriangleMesh::FaceType* adjF = f->FFp(bestEdge);
                int adjFI = f->FFi(bestEdge);

                if (!f->IsFaceEdgeS(bestEdge)) {
                    f->SetFaceEdgeS(bestEdge);
                    ++newEdgesSelected;
                }

                if (!adjF->IsFaceEdgeS(adjFI)) {
                    adjF->SetFaceEdgeS(adjFI);
                    ++newEdgesSelected;
                }
            }
        }

        std::cout << "After fixing curve on manifold bug: " << newEdgesSelected << " new edges selected. This number should be: " << edgeMesh.EN()*2 - totalEdgesSelected << "." << std::endl;
    }


    vcg::tri::UpdateSelection<VCGMesh>::FaceClear(resultMesh);
    vcg::tri::UpdateSelection<VCGMesh>::VertexClear(resultMesh);
    for (size_t i = 0; i < resultMesh.face.size(); ++i) {
        if (!resultMesh.face[i].IsD()) {
            for (int k = 0; k < resultMesh.face[i].VN(); ++k) {
                if (resultMesh.face[i].IsFaceEdgeS(k)) {
                    resultMesh.face[i].V0(k)->SetS();
                    resultMesh.face[i].V1(k)->SetS();
                    resultMesh.face[i].SetS();
                }
            }
        }
    }

    birthVertex.resize(resultMesh.vert.size(), MAX_INDEX);
    #pragma omp parallel for
    for (Index i = 0; i < resultMesh.vert.size(); ++i) {
        if (!resultMesh.vert[i].IsD() && !resultMesh.vert[i].IsS()) {
            assert(resultMesh.vert[i].Q() >= 0);
            birthVertex[i] = resultMesh.vert[i].Q();
        }
    }
    birthFace.resize(resultMesh.face.size(), MAX_INDEX);
    #pragma omp parallel for
    for (Index i = 0; i < resultMesh.face.size(); ++i) {
        if (!resultMesh.face[i].IsD()) {
            assert(resultMesh.face[i].Q() >= 0);
            birthFace[i] = resultMesh.face[i].Q();
        }
    }

    return done;
}

}
