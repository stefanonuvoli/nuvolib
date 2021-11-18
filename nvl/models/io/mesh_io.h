#ifndef NVL_MODELS_MESH_IO_H
#define NVL_MODELS_MESH_IO_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/mesh_io_data.h>
#include <nvl/models/io/mesh_io_obj.h>

#include <nvl/models/structures/vertex_mesh.h>
#include <nvl/models/structures/polyline_mesh.h>
#include <nvl/models/structures/face_mesh.h>

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
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors);

template<class V, class P, class N, class U, class C>
void meshLoadVertexData(
        VertexMesh<V>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors);

template<class V, class L, class P, class N, class U, class C>
void meshLoadVertexData(
        PolylineMesh<V,L>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors);

template<class V, class L, class F, class P, class N, class U, class C>
void meshLoadVertexData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors);

template<class Mesh, class C>
void meshLoadPolylineData(
        Mesh& mesh,
        const std::vector<std::vector<Index>>& polylines,
        const std::vector<C>& polylineColors);

template<class V, class L, class C>
void meshLoadPolylineData(
        PolylineMesh<V,L>& mesh,
        const std::vector<std::vector<Index>>& polylines,
        const std::vector<C>& polylineColors);

template<class V, class L, class F, class C>
void meshLoadPolylineData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<std::vector<Index>>& polylines,
        const std::vector<C>& polylineColors);

template<class Mesh, class FN, class VN, class U, class M>
void meshLoadFaceData(
        Mesh& mesh,
        const std::vector<std::vector<Index>>& faces,
        const std::vector<FN>& faceNormals,
        const std::vector<M>& materials,
        const std::vector<std::string>& faceMaterials,
        const std::vector<VN>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<std::vector<Index>>& faceVertexNormals,
        const std::vector<std::vector<Index>>& faceVertexUVs);

template<class V, class L, class F, class FN, class VN, class U, class M>
void meshLoadFaceData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<std::vector<Index>>& faces,
        const std::vector<FN>& faceNormals,
        const std::vector<M>& materials,
        const std::vector<std::string>& faceMaterials,
        const std::vector<VN>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<std::vector<Index>>& faceVertexNormals,
        const std::vector<std::vector<Index>>& faceVertexUVs);



/* Save raw data IO */


template<class Mesh, class P, class N, class U, class C>
void meshSaveVertexData(
        const Mesh& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors);

template<class V, class P, class N, class U, class C>
void meshSaveVertexData(
        const VertexMesh<V>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors);

template<class V, class L, class P, class N, class U, class C>
void meshSaveVertexData(
        const PolylineMesh<V,L>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors);

template<class V, class L, class F, class P, class N, class U, class C>
void meshSaveVertexData(
        const FaceMesh<V,L,F>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors);

template<class Mesh, class C>
void meshSavePolylineData(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& polylines,
        std::vector<C>& polylineColors);

template<class V, class P, class C>
void meshSavePolylineData(
        const PolylineMesh<V,P>& mesh,
        std::vector<std::vector<Index>>& polylines,
        std::vector<C>& polylineColors);

template<class V, class L, class F, class C>
void meshSavePolylineData(
        const FaceMesh<V,L,F>& mesh,
        std::vector<std::vector<Index>>& polylines,
        std::vector<C>& polylineColors);

template<class Mesh, class FN, class VN, class U, class M>
void meshSaveFaceData(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& faces,
        std::vector<FN>& faceNormals,
        std::vector<M>& materials,
        std::vector<std::string>& faceMaterials,
        std::vector<VN>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<std::vector<Index>>& faceVertexNormals,
        std::vector<std::vector<Index>>& faceVertexUVs);

template<class V, class P, class F, class FN, class VN, class U, class M>
void meshSaveFaceData(
        const FaceMesh<V,P,F>& mesh,
        std::vector<std::vector<Index>>& faces,
        std::vector<FN>& faceNormals,
        std::vector<M>& materials,
        std::vector<std::string>& faceMaterials,
        std::vector<VN>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<std::vector<Index>>& faceVertexNormals,
        std::vector<std::vector<Index>>& faceVertexUVs);

}

#include "mesh_io.cpp"

#endif // NVL_MODELS_MESH_IO_H
