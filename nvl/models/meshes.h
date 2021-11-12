#ifndef NVL_MODELS_MESHES_H
#define NVL_MODELS_MESHES_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>
#include <nvl/math/vector.h>

#include <nvl/models/vertex_mesh.h>
#include <nvl/models/polyline_mesh.h>
#include <nvl/models/face_mesh.h>
#include <nvl/models/edge_mesh.h>

#include <nvl/models/mesh_vertex.h>
#include <nvl/models/mesh_polyline.h>
#include <nvl/models/mesh_array_face.h>
#include <nvl/models/mesh_vector_face.h>
#include <nvl/models/mesh_edge.h>
#include <nvl/models/mesh_material.h>

#include <nvl/utilities/color.h>

namespace nvl {

/* Basic components */
typedef MeshVertex<Point3d, Vector3d, Point2f, Color> Vertex3d;

typedef MeshPolyline<Vertex3d, Color> Polyline3d;

typedef MeshMaterial<Color> MeshMaterial3d;
typedef MeshVectorFace<Vertex3d, Vector3d, MeshMaterial3d> PolygonFace3d;
typedef MeshArrayFace<Vertex3d, Vector3d, MeshMaterial3d, 3> TriangleFace3d;
typedef MeshArrayFace<Vertex3d, Vector3d, MeshMaterial3d, 4> QuadFace3d;

typedef MeshEdge<Vector3d, PolygonFace3d> PolygonEdge3d;
typedef MeshEdge<Vector3d, TriangleFace3d> TriangleEdge3d;
typedef MeshEdge<Vector3d, QuadFace3d> QuadEdge3d;

/* Vertex meshes */
typedef VertexMesh<Vertex3d> VertexMesh3d;

/* Polyline meshes */
typedef PolylineMesh<Vertex3d, Polyline3d> PolylineMesh3d;

/* Face meshes */
typedef FaceMesh<Vertex3d, Polyline3d, TriangleFace3d> TriangleMesh3d;
typedef FaceMesh<Vertex3d, Polyline3d, QuadFace3d> QuadMesh3d;
typedef FaceMesh<Vertex3d, Polyline3d, PolygonFace3d> PolygonMesh3d;

/* Face edge meshes */
typedef EdgeMesh<Vertex3d, Polyline3d, TriangleFace3d, TriangleEdge3d> TriangleEdgeMesh3d;
typedef EdgeMesh<Vertex3d, Polyline3d, QuadFace3d, QuadEdge3d> QuadEdgeMesh3d;
typedef EdgeMesh<Vertex3d, Polyline3d, PolygonFace3d, PolygonEdge3d> PolygonEdgeMesh3d;

}

#endif // NVL_MODELS_MESHES_H
