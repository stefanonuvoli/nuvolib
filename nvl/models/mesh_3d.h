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

/* Defining types */

typedef MeshVertex<Point3d> Vertex3d;
typedef VertexMeshTypes<Vertex3d, Vector3d, Color, Point2f> Vertex3dTypes;

typedef MeshPolyline<Vertex3d> Polyline3d;
typedef PolylineMeshTypes<Polyline3d, Color> Polyline3dTypes;

typedef MeshMaterial<Color> MeshMaterial3d;
typedef MeshVectorFace<Vertex3d> PolygonFace3d;
typedef MeshArrayFace<Vertex3d, 3> TriangleFace3d;
typedef MeshArrayFace<Vertex3d, 4> QuadFace3d;
typedef FaceMeshTypes<PolygonFace3d, Vector3d, MeshMaterial3d> PolygonFace3dTypes;
typedef FaceMeshTypes<QuadFace3d, Vector3d, MeshMaterial3d> QuadFace3dTypes;
typedef FaceMeshTypes<TriangleFace3d, Vector3d, MeshMaterial3d> TriangleFace3dTypes;

/* Vertex meshes */

typedef VertexMesh<Vertex3dTypes> VertexMesh3d;

/* Polyline meshes */

typedef PolylineMesh<Vertex3dTypes, Polyline3dTypes> PolylineMesh3d;

/* Face meshes */

typedef FaceMesh<Vertex3dTypes, Polyline3dTypes, PolygonFace3dTypes> PolygonMesh3d;
typedef FaceMesh<Vertex3dTypes, Polyline3dTypes, QuadFace3dTypes> QuadMesh3d;
typedef FaceMesh<Vertex3dTypes, Polyline3dTypes, TriangleFace3dTypes> TriangleMesh3d;

}

#endif // NVL_MODELS_MESHE_3D_H
