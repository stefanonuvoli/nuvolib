#ifndef NVL_MODELS_MESH_IO_H
#define NVL_MODELS_MESH_IO_H

#include <nvl/nuvolib.h>

#include <nvl/models/mesh_io_data.h>
#include <nvl/models/mesh_io_obj.h>

#include <nvl/models/vertex_mesh.h>
#include <nvl/models/polyline_mesh.h>
#include <nvl/models/face_mesh.h>
#include <nvl/models/edge_mesh.h>

namespace nvl {

template<class Mesh>
bool meshLoadFromFile(
        const std::string& filename,
        Mesh& mesh,
        IOMeshError& error = internal::dummyIOMeshError,
        IOMeshMode& mode = internal::dummyIOMeshMode);

template<class Mesh>
bool meshSaveToFile(
        const std::string& filename,
        const Mesh& mesh,
        IOMeshError& error = internal::dummyIOMeshError,
        const IOMeshMode& mode = internal::dummyIOMeshMode);


/* Load raw data IO */


template<class Mesh, class P, class N, class U, class C>
void meshLoadVertexData(
        Mesh& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors);

template<class V, class P, class N, class U, class C>
void meshLoadVertexData(
        VertexMesh<V>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors);

template<class V, class L, class P, class N, class U, class C>
void meshLoadVertexData(
        PolylineMesh<V,L>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors);

template<class V, class L, class F, class P, class N, class U, class C>
void meshLoadVertexData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors);

template<class V, class L, class F, class E, class P, class N, class U, class C>
void meshLoadVertexData(
        EdgeMesh<V,L,F,E>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors);

template<class Mesh>
void meshLoadPolylineData(
        Mesh& mesh,
        const std::vector<std::vector<Index>>& polylines);

template<class V, class L>
void meshLoadPolylineData(
        PolylineMesh<V,L>& mesh,
        const std::vector<std::vector<Index>>& polylines);

template<class V, class L, class F>
void meshLoadPolylineData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<std::vector<Index>>& polylines);

template<class V, class L, class F, class E>
void meshLoadPolylineData(
        EdgeMesh<V,L,F,E>& mesh,
        const std::vector<std::vector<Index>>& polylines);

template<class Mesh, class N, class U, class M>
void meshLoadFaceData(
        Mesh& mesh,
        const std::vector<std::vector<Index>>& faces,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<std::vector<Index>>& faceVertexNormals,
        const std::vector<std::vector<Index>>& faceVertexUV,
        const std::vector<M>& materials,
        const std::vector<std::string>& faceMaterials);

template<class V, class L, class F, class N, class U, class M>
void meshLoadFaceData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<std::vector<Index>>& faces,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<std::vector<Index>>& faceVertexNormals,
        const std::vector<std::vector<Index>>& faceVertexUV,
        const std::vector<M>& materials,
        const std::vector<std::string>& faceMaterials);

template<class V, class L, class F, class E, class N, class U, class M>
void meshLoadFaceData(
        EdgeMesh<V,L,F,E>& mesh,
        const std::vector<std::vector<Index>>& faces,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<std::vector<Index>>& faceVertexNormals,
        const std::vector<std::vector<Index>>& faceVertexUV,
        const std::vector<M>& materials,
        const std::vector<std::string>& faceMaterials);




/* Save raw data IO */


template<class Mesh, class V, class P, class N, class U, class C>
void meshSaveVertexData(
        const Mesh& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors);

template<class V, class P, class N, class U, class C>
void meshSaveVertexData(
        const VertexMesh<V>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors);

template<class V, class L, class P, class N, class U, class C>
void meshSaveVertexData(
        const PolylineMesh<V,L>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors);

template<class V, class L, class F, class P, class N, class U, class C>
void meshSaveVertexData(
        const FaceMesh<V,L,F>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors);

template<class V, class L, class F, class E, class P, class N, class U, class C>
void meshSaveVertexData(
        const EdgeMesh<V,L,F,E>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors);

template<class Mesh, class V>
void meshSavePolylineData(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& polylines);

template<class V, class P>
void meshSavePolylineData(
        const PolylineMesh<V,P>& mesh,
        std::vector<std::vector<Index>>& polylines);

template<class V, class L, class F>
void meshSavePolylineData(
        const FaceMesh<V,L,F>& mesh,
        std::vector<std::vector<Index>>& polylines);

template<class V, class L, class F, class E>
void meshSavePolylineData(
        const EdgeMesh<V,L,F,E>& mesh,
        std::vector<std::vector<Index>>& polylines);

template<class Mesh, class N, class U, class M>
void meshSaveFaceData(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& faces,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<std::vector<Index>>& faceVertexNormals,
        std::vector<std::vector<Index>>& faceVertexUV,
        std::vector<M>& materials,
        std::vector<std::string>& faceMaterials);

template<class V, class P, class F, class N, class U, class M>
void meshSaveFaceData(
        const FaceMesh<V,P,F>& mesh,
        std::vector<std::vector<Index>>& faces,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<std::vector<Index>>& faceVertexNormals,
        std::vector<std::vector<Index>>& faceVertexUV,
        std::vector<M>& materials,
        std::vector<std::string>& faceMaterials);

template<class V, class L, class F, class E, class N, class U, class M>
void meshSaveFaceData(
        const EdgeMesh<V,L,F,E>& mesh,
        std::vector<std::vector<Index>>& faces,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<std::vector<Index>>& faceVertexNormals,
        std::vector<std::vector<Index>>& faceVertexUV,
        std::vector<M>& materials,
        std::vector<std::string>& faceMaterials);

}

#include "mesh_io.cpp"

#endif // NVL_MODELS_MESH_IO_H
