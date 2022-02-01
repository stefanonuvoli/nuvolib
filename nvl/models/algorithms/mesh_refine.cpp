/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_refine.h"

#ifdef NVL_VCGLIB_LOADED

#include <nvl/math/comparisons.h>

#include <nvl/models/structures/vcg_triangle_mesh.h>
#include <nvl/models/algorithms/mesh_vcg_convert.h>
#include <nvl/models/algorithms/mesh_triangulation.h>

#include <vcg/complex/algorithms/refine.h>
#include <vcg/complex/algorithms/update/quality.h>

#include <set>

namespace nvl {

template<class Mesh, class F>
Mesh refineByImplicitFunction(
        const Mesh& mesh,
        const std::vector<F>& vertexFunction,
        std::vector<std::pair<typename Mesh::VertexId, typename Mesh::VertexId>>& curveLines,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace)
{
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;

    Mesh outputMesh;

    curveLines.clear();
    birthVertex.clear();
    birthFace.clear();

    Mesh triangulatedMesh = mesh;
    std::vector<FaceId> triangulatedBirthFace;
    meshTriangulateConvexFace(triangulatedMesh, triangulatedBirthFace);

    VCGTriangleMesh vcgMesh, vcgResultMesh;

    std::vector<VertexId> vcgBirthVertex;
    std::vector<FaceId> vcgBirthFace;
    convertMeshToVCGMesh(triangulatedMesh, vcgMesh, vcgBirthVertex, vcgBirthFace);

    std::vector<std::pair<Index, Index>> vcgCurveVertices;

    std::vector<typename Mesh::VertexId> refineBirthVertex;
    std::vector<typename Mesh::FaceId> refineBirthFace;
    vcgRefineByImplicitFunction(vcgMesh, vertexFunction, vcgResultMesh, vcgCurveVertices, refineBirthVertex, refineBirthFace);

    if (vcgCurveVertices.size() < 3) {
        meshTransferFaces(mesh, outputMesh, birthVertex, birthFace);
        return outputMesh;
    }

    for (const std::pair<Index, Index>& value : vcgCurveVertices) {
        const Index& v1 = value.first;
        const Index& v2 = value.second;

        curveLines.push_back(std::make_pair(std::min(v1, v2), std::max(v1, v2)));
    }

    Mesh triangulatedResultMesh;
    std::vector<Index> triangulatedResultBirthVertex;
    std::vector<Index> triangulatedResultBirthFace;
    convertVCGMeshToMesh(vcgResultMesh, triangulatedResultMesh, triangulatedResultBirthVertex, triangulatedResultBirthFace);

    birthVertex.resize(triangulatedResultMesh.nextVertexId(), NULL_ID);
    for (VertexId vId = 0; vId < triangulatedResultMesh.nextVertexId(); ++vId) {
        if (!triangulatedResultMesh.isVertexDeleted(vId)) {
            Index mapping;

            mapping = triangulatedResultBirthVertex[vId];
            assert(mapping != NULL_ID);
            mapping = refineBirthVertex[mapping];
            if (mapping == NULL_ID)
                continue;
            mapping = vcgBirthVertex[mapping];
            assert(mapping != NULL_ID);

            birthVertex[vId] = mapping;
        }
    }

    std::vector<FaceId> tmpResultBirthFace(triangulatedResultMesh.nextFaceId(), NULL_ID);
    for (FaceId fId = 0; fId < triangulatedResultMesh.nextFaceId(); ++fId) {
        if (!triangulatedResultMesh.isFaceDeleted(fId)) {
            Index mapping;

            mapping = triangulatedResultBirthFace[fId];
            assert(mapping != NULL_ID);
            mapping = refineBirthFace[mapping];
            assert(mapping != NULL_ID);
            mapping = vcgBirthFace[mapping];
            assert(mapping != NULL_ID);
            mapping = triangulatedBirthFace[mapping];
            assert(mapping != NULL_ID);

            tmpResultBirthFace[fId] = mapping;
        }
    }

    outputMesh = meshReconstructTriangulatedPolygons(mesh, triangulatedResultMesh, birthVertex, tmpResultBirthFace, birthFace);

    return outputMesh;
}

template<class VCGMesh, class F>
bool vcgRefineByImplicitFunction(
        VCGMesh& mesh,        
        const std::vector<F>& vertexFunction,
        VCGMesh& resultMesh,
        std::vector<std::pair<Index, Index>>& curveLines,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace)
{
    typedef typename VCGMesh::VertexType VertexType;
    typedef typename VCGMesh::ScalarType ScalarType;
    typedef typename VCGMesh::CoordType CoordType;
    typedef std::pair<CoordType, CoordType> EdgeKey;

    std::map<EdgeKey,CoordType> splitMap;
    for (size_t i = 0; i < mesh.face.size(); ++i) {
        if (mesh.face[i].IsD())
            continue;

        for (int j = 0; j < mesh.face[i].VN(); ++j) {
            VertexType* v1 = mesh.face[i].V0(j);
            VertexType* v2 = mesh.face[i].V1(j);

            ScalarType f1 = vertexFunction[vcg::tri::Index(mesh, v1)];
            ScalarType f2 = vertexFunction[vcg::tri::Index(mesh, v2)];

            CoordType p0 = v1->P();
            CoordType p1 = v2->P();
            std::pair<CoordType,CoordType> key(std::min(p0, p1), std::max(p0, p1));

            if (epsEqual(f1, 0.0) || epsEqual(f2, 0.0)) {
                continue;
            }

            if (f1 * f2 < 0) {
                ScalarType sum = fabs(f1) + fabs(f2);
                ScalarType w = 1 - (fabs(f1) / sum);

                splitMap[key] = p0 * w + p1 * (1 - w);
            }
        }
    }

    bool done = vcgRefineMesh(mesh, splitMap, resultMesh, curveLines, birthVertex, birthFace);

    return done;
}

template<class VCGMesh>
bool vcgRefineMesh(
        VCGMesh& mesh,
        const std::map<std::pair<typename VCGMesh::CoordType, typename VCGMesh::CoordType>, typename VCGMesh::CoordType>& splitMap,
        VCGMesh& resultMesh,
        std::vector<std::pair<Index, Index>>& curveLines,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace)
{
    typedef typename VCGMesh::VertexType VertexType;
    typedef typename VCGMesh::FaceType FaceType;
    typedef typename VCGMesh::CoordType CoordType;
    typedef std::pair<CoordType, CoordType> EdgeKey;
    typedef vcg::face::Pos<FaceType> Pos;

    struct SplitLev : public std::unary_function<Pos, CoordType>
    {
        const std::map<EdgeKey, CoordType>& splitMap;

        void operator()(VertexType& nv, Pos& ep) const
        {
            VertexType* v0 = ep.f->V0(ep.z);
            VertexType* v1 = ep.f->V1(ep.z);

            assert(v0 != v1);

            CoordType pos1 = v0->P();
            CoordType pos2 = v1->P();

            EdgeKey coord(std::min(pos1,pos2), std::max(pos1,pos2));

            assert(splitMap.find(coord) != splitMap.end());
            nv.P() = splitMap.at(coord);
            nv.SetS();
        }

        vcg::TexCoord2f WedgeInterp(
                vcg::TexCoord2f& t0,
                vcg::TexCoord2f& t1)
        {
            (void) t0;
            (void) t1;
            return (vcg::TexCoord2f(0,0));
        }

        SplitLev(const std::map<EdgeKey,CoordType>& splitMap) :
            splitMap(splitMap)
        {

        }
    };
    struct EdgePred
    {
        const std::map<EdgeKey, CoordType>& splitMap;

        bool operator()(Pos& ep) const
        {
            VertexType* v0 = ep.f->V0(ep.z);
            VertexType* v1 = ep.f->V1(ep.z);

            assert(v0 != v1);

            CoordType pos1 = v0->P();
            CoordType pos2 = v1->P();

            EdgeKey coord(std::min(pos1, pos2), std::max(pos1, pos2));

            return splitMap.find(coord) != splitMap.end();
        }

        EdgePred(const std::map<EdgeKey, CoordType>& splitMap) :
            splitMap(splitMap)
        {

        }
    };

    SplitLev sp(splitMap);
    EdgePred ep(splitMap);

    return vcgRefineMesh(mesh, sp, ep, resultMesh, curveLines, birthVertex, birthFace);
}



template<class VCGMesh, class SplitLev, class EdgePred>
bool vcgRefineMesh(
        VCGMesh& mesh,
        const SplitLev& sp,
        const EdgePred& ep,
        VCGMesh& resultMesh,
        std::vector<std::pair<Index, Index>>& curveLines,
        std::vector<Index>& birthVertex,
        std::vector<Index>& birthFace)
{
    typedef typename VCGMesh::VertexType VertexType;
    typedef typename VCGMesh::CoordType CoordType;

    vcg::tri::Append<VCGMesh, VCGMesh>::Mesh(resultMesh, mesh);

    vcg::tri::UpdateNormal<VCGMesh>::PerFaceNormalized(resultMesh);
    vcg::tri::UpdateNormal<VCGMesh>::PerVertexNormalized(resultMesh);
    vcg::tri::UpdateBounding<VCGMesh>::Box(resultMesh);
    vcg::tri::UpdateTopology<VCGMesh>::FaceFace(resultMesh);
    vcg::tri::UpdateTopology<VCGMesh>::VertexFace(resultMesh);
    vcg::tri::UpdateFlags<VCGMesh>::FaceBorderFromFF(resultMesh);
    vcg::tri::UpdateFlags<VCGMesh>::VertexBorderFromNone(resultMesh);
    vcg::tri::UpdateSelection<VCGMesh>::VertexClear(resultMesh);

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

    bool done = vcg::tri::RefineE<VCGMesh, SplitLev, EdgePred>(resultMesh, sp, ep);

    vcg::tri::UpdateSelection<VCGMesh>::FaceClear(resultMesh);
    for (size_t i = 0; i < resultMesh.face.size(); ++i) {
        if (!resultMesh.face[i].IsD()) {
            for (int k = 0; k < resultMesh.face[i].VN(); ++k) {
                VertexType* v1 = resultMesh.face[i].V0(k);
                VertexType* v2 = resultMesh.face[i].V1(k);

                const Index vId1 = vcg::tri::Index(resultMesh, resultMesh.face[i].V0(k));
                const Index vId2 = vcg::tri::Index(resultMesh, resultMesh.face[i].V1(k));

                if (v1->IsS() || v2->IsS()) {
                    resultMesh.face[i].SetS();
                }

                if (v1->IsS() && v2->IsS()) {
                    const CoordType& p1 = resultMesh.face[i].V0(k)->cP();
                    const CoordType& p2 = resultMesh.face[i].V1(k)->cP();

                    std::pair<CoordType, CoordType> newEdge;
                    newEdge.first = CoordType(p1.X(), p1.Y(), p1.Z());
                    newEdge.second = CoordType(p2.X(), p2.Y(), p2.Z());

                    if (newEdge.first < newEdge.second)
                        std::swap(newEdge.first, newEdge.second);

                    curveLines.push_back(std::make_pair(std::min(vId1, vId2), std::max(vId1, vId2)));
                }
            }
        }
    }

    //Sort and erase duplicates
    std::sort(curveLines.begin(), curveLines.end());
    curveLines.erase(std::unique(curveLines.begin(), curveLines.end()), curveLines.end());

    //Setting the birth information
    birthVertex.resize(resultMesh.vert.size(), NULL_ID);
    #pragma omp parallel for
    for (Index i = 0; i < resultMesh.vert.size(); ++i) {
        if (!resultMesh.vert[i].IsD() && !resultMesh.vert[i].IsS()) {
            assert(resultMesh.vert[i].Q() >= 0);
            birthVertex[i] = resultMesh.vert[i].Q();
        }
    }

    birthFace.resize(resultMesh.face.size(), NULL_ID);
    #pragma omp parallel for
    for (Index i = 0; i < resultMesh.face.size(); ++i) {
        if (!resultMesh.face[i].IsD()) {
            assert(resultMesh.face[i].Q() >= 0);
            birthFace[i] = resultMesh.face[i].Q();
        }
    }


    return done;
}

#endif

}
