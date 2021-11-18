#ifndef NVL_MODELS_MESHE_3D_H
#define NVL_MODELS_MESHE_3D_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>
#include <nvl/math/vector.h>

#include <nvl/models/structures/vertex_mesh.h>
#include <nvl/models/structures/polyline_mesh.h>
#include <nvl/models/structures/face_mesh.h>

#include <nvl/models/structures/mesh_vertex.h>
#include <nvl/models/structures/mesh_polyline.h>
#include <nvl/models/structures/mesh_array_face.h>
#include <nvl/models/structures/mesh_vector_face.h>
#include <nvl/models/structures/mesh_material.h>

#include <nvl/utilities/color.h>

namespace nvl {

/* Basic components */
typedef MeshVertex<Point3d, Vector3d, Point2f, Color> Vertex3d;

typedef MeshPolyline<Vertex3d, Color> Polyline3d;

typedef MeshMaterial<Color> MeshMaterial3d;
typedef MeshVectorFace<Vertex3d, Vector3d, MeshMaterial3d> PolygonFace3d;
typedef MeshArrayFace<Vertex3d, Vector3d, MeshMaterial3d, 3> TriangleFace3d;
typedef MeshArrayFace<Vertex3d, Vector3d, MeshMaterial3d, 4> QuadFace3d;

/* Vertex meshes */
typedef VertexMesh<Vertex3d> VertexMesh3d;

/* Polyline meshes */
typedef PolylineMesh<Vertex3d, Polyline3d> PolylineMesh3d;

/* Face meshes */
typedef FaceMesh<Vertex3d, Polyline3d, TriangleFace3d> TriangleMesh3d;
typedef FaceMesh<Vertex3d, Polyline3d, QuadFace3d> QuadMesh3d;
typedef FaceMesh<Vertex3d, Polyline3d, PolygonFace3d> PolygonMesh3d;

}

#endif // NVL_MODELS_MESHE_3D_H
