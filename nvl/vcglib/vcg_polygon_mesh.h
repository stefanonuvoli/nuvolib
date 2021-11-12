#ifndef NVL_VCGLIB_VCG_POLYGON_MESH_H
#define NVL_VCGLIB_VCG_POLYGON_MESH_H

#include <vcg/complex/complex.h>

namespace nvl {

/* ----- Polygon mesh ----- */

class VCGPolygonVertex;
class VCGPolygonFace;
class VCGPolygonEdge;

struct VCGPolygonTypes : public vcg::UsedTypes<
        vcg::Use<VCGPolygonVertex>::AsVertexType,
        vcg::Use<VCGPolygonEdge>::AsEdgeType,
        vcg::Use<VCGPolygonFace>::AsFaceType>{};

class VCGPolygonVertex : public vcg::Vertex<VCGPolygonTypes,
        vcg::vertex::Coord3d,
        vcg::vertex::Normal3d,
        vcg::vertex::Color4b,
        vcg::vertex::Qualityd,
        vcg::vertex::BitFlags,
        vcg::vertex::VFAdj,
        vcg::vertex::CurvatureDird,
        vcg::vertex::Mark>{};

class VCGPolygonFace : public vcg::Face<
        VCGPolygonTypes,
        vcg::face::PolyInfo,
        vcg::face::VertexRef,
        vcg::face::Normal3d,
        vcg::face::Color4b,
        vcg::face::Qualityd,
        vcg::face::BitFlags,
        vcg::face::PFVAdj,
        vcg::face::PFFAdj,
        vcg::face::PVFAdj,
        vcg::face::CurvatureDird,
        vcg::face::Mark> {};

class VCGPolygonEdge : public vcg::Edge<
        VCGPolygonTypes,
        vcg::edge::VertexRef,
        vcg::edge::VEAdj,
        vcg::edge::EEAdj,
        vcg::edge::BitFlags> {};

class VCGPolygonMesh : public vcg::tri::TriMesh<
        std::vector<VCGPolygonVertex>,
        std::vector<VCGPolygonEdge>,
        std::vector<VCGPolygonFace>> {};

}

#endif // NVL_VCGLIB_VCG_POLYGON_MESH_H
