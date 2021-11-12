#ifndef NVL_VCGLIB_VCG_TRIANGLE_MESH_H
#define NVL_VCGLIB_VCG_TRIANGLE_MESH_H

#include <vcg/complex/complex.h>

namespace nvl {


/* ----- Triangle edge mesh ----- */

class VCGTriangleVertex;
class VCGTriangleFace;
class VCGTriangleEdge;

struct VCGTriangleTypes : public vcg::UsedTypes<
        vcg::Use<VCGTriangleVertex>::AsVertexType,
        vcg::Use<VCGTriangleEdge>::AsEdgeType,
        vcg::Use<VCGTriangleFace>::AsFaceType>{};

class VCGTriangleVertex : public vcg::Vertex<
        VCGTriangleTypes,
        vcg::vertex::Coord3d,
        vcg::vertex::Normal3d,
        vcg::vertex::VFAdj,
        vcg::vertex::VEAdj,
        vcg::vertex::Color4b,
        vcg::vertex::Qualityd,
        vcg::vertex::BitFlags,
        vcg::vertex::CurvatureDird,
        vcg::vertex::Mark,
        vcg::vertex::TexCoord2f>{};

class VCGTriangleEdge : public vcg::Edge<
        VCGTriangleTypes,
        vcg::edge::VertexRef,
        vcg::edge::VEAdj,
        vcg::edge::EEAdj,
        vcg::edge::BitFlags> {};

class VCGTriangleFace : public vcg::Face<
        VCGTriangleTypes,
        vcg::face::VertexRef,
        vcg::face::Normal3d,
        vcg::face::Color4b,
        vcg::face::Qualityd,
        vcg::face::BitFlags,
        vcg::face::FFAdj,
        vcg::face::VFAdj,
        vcg::face::CurvatureDird,
        vcg::face::Mark,
        vcg::face::WedgeTexCoord2f> {};

class VCGTriangleMesh : public vcg::tri::TriMesh<
        std::vector<VCGTriangleVertex>,
        std::vector<VCGTriangleEdge>,
        std::vector<VCGTriangleFace> > {};

}

#endif // NVL_VCGLIB_VCG_TRIANGLE_MESH_H
