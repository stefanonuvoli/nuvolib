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

template<class VT, class P, class N, class U, class C>
void meshLoadVertexData(
        VertexMesh<VT>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors);

template<class VT, class PT, class P, class N, class U, class C>
void meshLoadVertexData(
        PolylineMesh<VT,PT>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors);

template<class VT, class PT, class FT, class P, class N, class U, class C>
void meshLoadVertexData(
        FaceMesh<VT,PT,FT>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors);

template<class Mesh, class C>
void meshLoadPolylineData(
        Mesh& mesh,
        const std::vector<std::vector<Index>>& polylines,
        const std::vector<C>& polylineColors);

template<class VT, class PT, class C>
void meshLoadPolylineData(
        PolylineMesh<VT,PT>& mesh,
        const std::vector<std::vector<Index>>& polylines,
        const std::vector<C>& polylineColors);

template<class VT, class PT, class FT, class C>
void meshLoadPolylineData(
        FaceMesh<VT,PT,FT>& mesh,
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

template<class VT, class PT, class FT, class FN, class VN, class U, class M>
void meshLoadFaceData(
        FaceMesh<VT,PT,FT>& mesh,
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

template<class VT, class P, class N, class U, class C>
void meshSaveVertexData(
        const VertexMesh<VT>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors);

template<class VT, class PT, class P, class N, class U, class C>
void meshSaveVertexData(
        const PolylineMesh<VT,PT>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors);

template<class VT, class PT, class FT, class P, class N, class U, class C>
void meshSaveVertexData(
        const FaceMesh<VT,PT,FT>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors);

template<class Mesh, class C>
void meshSavePolylineData(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& polylines,
        std::vector<C>& polylineColors);

template<class VT, class PT, class C>
void meshSavePolylineData(
        const PolylineMesh<VT,PT>& mesh,
        std::vector<std::vector<Index>>& polylines,
        std::vector<C>& polylineColors);

template<class VT, class PT, class FT, class C>
void meshSavePolylineData(
        const FaceMesh<VT,PT,FT>& mesh,
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

template<class VT, class PT, class FT, class FN, class VN, class U, class M>
void meshSaveFaceData(
        const FaceMesh<VT,PT,FT>& mesh,
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
