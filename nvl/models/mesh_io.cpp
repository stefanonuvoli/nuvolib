#include "mesh_io.h"

#include <nvl/utilities/file_utils.h>
#include <unordered_map>

namespace nvl {

template<class Mesh>
bool meshLoadFromFile(
        const std::string& filename,
        Mesh& mesh,
        IOMeshError& error,
        IOMeshMode& mode)
{
    IOMeshData<
            typename Mesh::Point,
            typename Mesh::VertexNormal,
            typename Mesh::UV,
            typename Mesh::VertexColor,
            typename Mesh::FaceNormal,
            typename Mesh::Material> data;

    std::string ext = filenameExtension(filename);

    mesh.clear();

    bool success;
    if (ext == "obj") {
        success = meshLoadDataFromObj(filename, data, error, mode);
    }
    else {
        error = IO_MESH_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        meshLoadVertexData(mesh, data.vertices, data.vertexNormals, data.vertexUV, data.vertexColors);
        meshLoadPolylineData(mesh, data.polylines);
        meshLoadFaceData(mesh, data.faces, data.vertexNormals, data.vertexUV, data.faceVertexNormals, data.faceVertexUV, data.materials, data.faceMaterials);
    }

    return success;
}

template<class Mesh>
bool meshSaveToFile(
        const std::string& filename,
        const Mesh& mesh,
        IOMeshError& error,
        const IOMeshMode& mode)
{
    std::string ext = filenameExtension(filename);

    bool success;
    if (ext == "obj") {
        IOMeshData<
                typename Mesh::Point,
                typename Mesh::VertexNormal,
                typename Mesh::UV,
                typename Mesh::VertexColor,
                typename Mesh::FaceNormal,
                typename Mesh::Material> data;

        meshSaveVertexData(mesh, data.vertices, data.vertexNormals, data.vertexUV, data.vertexColors);
        meshSavePolylineData(mesh, data.polylines);
        meshSaveFaceData(mesh, data.faces, data.vertexNormals, data.vertexUV, data.faceVertexNormals, data.faceVertexUV, data.materials, data.faceMaterials);

        success = meshSaveDataToObj(filename, data, error, mode);
    }
    else {
        error = IO_MESH_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}

template<class Mesh, class P, class N, class U, class C>
void meshLoadVertexData(
        Mesh& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertices);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexUV);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexColors);
}

template<class V, class P, class N, class U, class C>
void meshLoadVertexData(
        VertexMesh<V>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors)
{
    typedef typename VertexMesh<V>::VertexId VertexId;

    for (Index i = 0; i < vertices.size(); ++i) {
        const P& point = vertices[i];

        VertexId vId = mesh.addVertex(point);
        assert(i == vId);

        if (vertexNormals.size() == vertices.size()) {
            mesh.setVertexNormal(vId, vertexNormals[i]);
        }

        if (vertexUV.size() == vertices.size()) {
            mesh.setVertexUV(vId, vertexUV[i]);
        }

        if (vertexColors.size() == vertices.size()) {
            mesh.setVertexColor(vId, vertexColors[i]);
        }
    }
}

template<class V, class L, class P, class N, class U, class C>
void meshLoadVertexData(
        PolylineMesh<V,L>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors)
{
    meshLoadVertexData(static_cast<VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUV, vertexColors);
}

template<class V, class L, class F, class P, class N, class U, class C>
void meshLoadVertexData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors)
{
    meshLoadVertexData(static_cast<VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUV, vertexColors);
}

template<class V, class L, class F, class E, class P, class N, class U, class C>
void meshLoadVertexData(
        EdgeMesh<V,L,F,E>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<C>& vertexColors)
{
    meshLoadVertexData(static_cast<VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUV, vertexColors);
}

template<class Mesh>
void meshLoadPolylineData(
        Mesh& mesh,
        const std::vector<std::vector<Index>>& polylines)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(polylines);
}

template<class V, class L>
void meshLoadPolylineData(
        PolylineMesh<V,L>& mesh,
        const std::vector<std::vector<Index>>& polylines)
{
    for (const std::vector<Index>& polyline : polylines) {
        mesh.addPolyline(polyline);
    }
}

template<class V, class L, class F>
void meshLoadPolylineData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<std::vector<Index>>& polylines)
{
    meshLoadPolylineData(static_cast<PolylineMesh<V,L>&>(mesh), polylines);
}

template<class V, class L, class F, class E>
void meshLoadPolylineData(
        EdgeMesh<V,L,F,E>& mesh,
        const std::vector<std::vector<Index>>& polylines)
{
    meshLoadPolylineData(static_cast<PolylineMesh<V,L>&>(mesh), polylines);
}

template<class Mesh, class N, class U, class M>
void meshLoadFaceData(
        Mesh& mesh,
        const std::vector<std::vector<Index>>& faces,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<std::vector<Index>>& faceVertexNormals,
        const std::vector<std::vector<Index>>& faceVertexUV,
        const std::vector<M>& materials,
        const std::vector<std::string>& faceMaterials)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(faces);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexUV);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceVertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceVertexUV);
    NVL_SUPPRESS_UNUSEDVARIABLE(materials);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceMaterials);
}

template<class V, class L, class F, class N, class U, class M>
void meshLoadFaceData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<std::vector<Index>>& faces,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<std::vector<Index>>& faceVertexNormals,
        const std::vector<std::vector<Index>>& faceVertexUV,
        const std::vector<M>& materials,
        const std::vector<std::string>& faceMaterials)
{
    typedef typename FaceMesh<V,L,F>::MaterialId MaterialId;
    typedef typename FaceMesh<V,L,F>::FaceId FaceId;
    typedef typename FaceMesh<V,L,F>::VertexId VertexId;
    typedef typename FaceMesh<V,L,F>::VertexNormal VertexNormal;
    typedef typename FaceMesh<V,L,F>::UV UV;

    const Index NO_VALUE = nvl::MAX_INDEX;
    const Index MULTIPLE_VALUES = nvl::MAX_INDEX - 1;

    std::vector<Index> vertexToNormal(mesh.nextVertexId(), NO_VALUE);
    std::vector<Index> vertexToUV(mesh.nextVertexId(), NO_VALUE);

    if (!faceVertexNormals.empty()) {
        for (Index i = 0; i < faces.size(); ++i) {
            const std::vector<Index>& faceVertices = faces[i];

            for (Index j = 0; j < faceVertexNormals[i].size(); ++j) {
                const VertexId& vId = faceVertices[j];
                const Index& nId = faceVertexNormals[i][j];

                Index& mappedNormal = vertexToNormal[vId];
                if (mappedNormal == NO_VALUE) {
                    mappedNormal = nId;
                }
                else if (mappedNormal != nId) {
                    mappedNormal = MULTIPLE_VALUES;
                }
            }
        }

        for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
            if (vertexToNormal[vId] != MULTIPLE_VALUES && vertexToNormal[vId] != NO_VALUE) {
                mesh.setVertexNormal(vId, vertexNormals[vertexToNormal[vId]]);
            }
        }
    }

    if (!faceVertexUV.empty()) {
        for (Index i = 0; i < faces.size(); ++i) {
            const std::vector<Index>& faceVertices = faces[i];

            for (Index j = 0; j < faceVertexUV[i].size(); ++j) {
                const VertexId& vId = faceVertices[j];
                const Index& uId = faceVertexUV[i][j];

                Index& mappedUV = vertexToUV[vId];
                if (mappedUV == NO_VALUE) {
                    mappedUV = uId;
                }
                else if (mappedUV != uId) {
                    mappedUV = MULTIPLE_VALUES;
                }
            }
        }

        for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
            if (vertexToUV[vId] != MULTIPLE_VALUES && vertexToUV[vId] != NO_VALUE) {
                mesh.setVertexUV(vId, vertexUV[vertexToUV[vId]]);
            }
        }
    }

    for (Index i = 0; i < faces.size(); ++i) {
        const std::vector<Index>& faceVertices = faces[i];

        FaceId fId = mesh.addFace(faceVertices);

        if (!faceVertexNormals.empty()) {
            bool customVertexNormals = false;

            for (Index j = 0; j < faceVertexNormals[i].size(); ++j) {
                const VertexId& vId = faceVertices[j];
                if (vertexToNormal[vId] == MULTIPLE_VALUES || vertexToNormal[vId] == NO_VALUE) {
                    customVertexNormals = true;
                }
            }

            if (customVertexNormals) {
                std::vector<VertexNormal> wedgeNormals(faceVertexNormals[i].size());

                for (Index j = 0; j < wedgeNormals.size(); ++j) {
                    const Index& nId = faceVertexNormals[i][j];

                    wedgeNormals[j] = vertexNormals[nId];
                }

                mesh.setFaceWedgeVertexNormals(fId, wedgeNormals);
            }
        }

        if (!faceVertexUV.empty()) {
            bool customVertexUVs = false;

            for (Index j = 0; j < faceVertexUV[i].size(); ++j) {
                const VertexId& vId = faceVertices[j];
                if (vertexToUV[vId] == MULTIPLE_VALUES || vertexToUV[vId] == NO_VALUE) {
                    customVertexUVs = true;
                }
            }

            if (customVertexUVs) {
                std::vector<UV> wedgeUVs(faceVertexUV[i].size());

                for (Index j = 0; j < wedgeUVs.size(); ++j) {
                    const Index& nId = faceVertexUV[i][j];

                    wedgeUVs[j] = vertexUV[nId];
                }

                mesh.setFaceWedgeUV(fId, wedgeUVs);
            }
        }
    }

    std::map<std::string, MaterialId> materialMap;

    for (const M& material : materials) {
         MaterialId mId = mesh.addMaterial(material);
         materialMap.insert(std::make_pair(material.name(), mId));
    }

    for (Index i = 0; i < faceMaterials.size(); ++i) {
        const std::string& name = faceMaterials[i];
        if (!name.empty() && materialMap.find(name) != materialMap.end()) {
            mesh.face(i).setMaterialId(materialMap.at(name));
        }
    }
}

template<class V, class L, class F, class E, class N, class U, class M>
void meshLoadFaceData(
        EdgeMesh<V,L,F,E>& mesh,
        const std::vector<std::vector<Index>>& faces,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUV,
        const std::vector<std::vector<Index>>& faceVertexNormals,
        const std::vector<std::vector<Index>>& faceVertexUV,
        const std::vector<M>& materials,
        const std::vector<std::string>& faceMaterials)
{
    meshLoadFaceData(static_cast<FaceMesh<V,L,F>&>(mesh), faces, vertexNormals, vertexUV, faceVertexNormals, faceVertexUV, materials, faceMaterials);
}

template<class Mesh, class V, class P, class N, class U, class C>
void meshSaveVertexData(
        const Mesh& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertices);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexUV);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexColors);
}

template<class V, class P, class N, class U, class C>
void meshSaveVertexData(
        const VertexMesh<V>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors)
{
    typedef typename VertexMesh<V>::Vertex Vertex;

    for (const Vertex& v : mesh.vertices()) {
        vertices.push_back(v.point());
        vertexNormals.push_back(v.normal());
        vertexUV.push_back(v.uvCoords());
        vertexColors.push_back(v.color());
    }
}

template<class V, class L, class P, class N, class U, class C>
void meshSaveVertexData(
        const PolylineMesh<V,L>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors)
{
    meshSaveVertexData(static_cast<const VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUV, vertexColors);
}

template<class V, class L, class F, class P, class N, class U, class C>
void meshSaveVertexData(
        const FaceMesh<V,L,F>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors)
{
    meshSaveVertexData(static_cast<const VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUV, vertexColors);
}

template<class V, class L, class F, class E, class P, class N, class U, class C>
void meshSaveVertexData(
        const EdgeMesh<V,L,F,E>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<C>& vertexColors)
{
    meshSaveVertexData(static_cast<const VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUV, vertexColors);
}

template<class Mesh, class V>
void meshSavePolylineData(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& polylines)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(polylines);
}

template<class V, class P>
void meshSavePolylineData(
        const PolylineMesh<V,P>& mesh,
        std::vector<std::vector<Index>>& polylines)
{
    typedef typename PolylineMesh<V,P>::Polyline Polyline;

    for (const Polyline& p : mesh.polylines()) {
        polylines.push_back(p.vertexIds());
    }
}

template<class V, class L, class F>
void meshSavePolylineData(
        const FaceMesh<V,L,F>& mesh,
        std::vector<std::vector<Index>>& polylines)
{
    meshSavePolylineData(static_cast<const PolylineMesh<V,L>&>(mesh), polylines);
}

template<class V, class L, class F, class E>
void meshSavePolylineData(
        const EdgeMesh<V,L,F,E>& mesh,
        std::vector<std::vector<Index>>& polylines)
{
    meshSavePolylineData(static_cast<const PolylineMesh<V,L>&>(mesh), polylines);
}

template<class Mesh, class N, class U, class M>
void meshSaveFaceData(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& faces,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<std::vector<Index>>& faceVertexNormals,
        std::vector<std::vector<Index>>& faceVertexUV,
        std::vector<M>& materials,
        std::vector<std::string>& faceMaterials)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(faces);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexUV);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceVertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceVertexUV);
    NVL_SUPPRESS_UNUSEDVARIABLE(materials);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceMaterials);
}
template<class V, class P, class F, class N, class U, class M>
void meshSaveFaceData(
        const FaceMesh<V,P,F>& mesh,
        std::vector<std::vector<Index>>& faces,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<std::vector<Index>>& faceVertexNormals,
        std::vector<std::vector<Index>>& faceVertexUV,
        std::vector<M>& materials,
        std::vector<std::string>& faceMaterials)
{
    typedef typename FaceMesh<V,P,F>::Face Face;
    typedef typename FaceMesh<V,P,F>::VertexNormal VertexNormal;
    typedef typename FaceMesh<V,P,F>::UV UV;
    typedef typename Face::Container FaceContainer;
    typedef typename FaceMesh<V,P,F>::Material Material;

    for (const Face& f : mesh.faces()) {
        const FaceContainer& c = f.vertexIds();

        faces.push_back(std::vector<Index>(c.begin(), c.end()));

        if (mesh.hasFaceWedgeVertexNormals(f)) {
            std::vector<Index> normalVec;

            for (const VertexNormal& normal : mesh.faceWedgeVertexNormals(f)) {
                vertexNormals.push_back(normal);
                normalVec.push_back(vertexNormals.size() - 1);
            }

            faceVertexNormals.push_back(normalVec);
        }
        else {
            faceVertexNormals.push_back(std::vector<Index>(c.begin(), c.end()));
        }

        if (mesh.hasFaceWedgeUV(f)) {
            std::vector<Index> uvCoordsVec;

            for (const UV& uv : mesh.faceWedgeUV(f)) {
                vertexUV.push_back(uv);
                uvCoordsVec.push_back(vertexUV.size() - 1);
            }

            faceVertexUV.push_back(uvCoordsVec);
        }
        else {
            faceVertexUV.push_back(std::vector<Index>(c.begin(), c.end()));
        }
    }
    for (const Material& material : mesh.materials()) {
         materials.push_back(material);
    }
    for (const Face& f : mesh.faces()) {
        if (mesh.hasFaceMaterial(f)) {
            std::string name = mesh.faceMaterial(f).name();
            faceMaterials.push_back(name);
        }
        else {
            faceMaterials.push_back("");
        }
    }
}

template<class V, class L, class F, class E, class N, class U, class M>
void meshSaveFaceData(
        const EdgeMesh<V,L,F,E>& mesh,
        std::vector<std::vector<Index>>& faces,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUV,
        std::vector<std::vector<Index>>& faceVertexNormals,
        std::vector<std::vector<Index>>& faceVertexUV,
        std::vector<M>& materials,
        std::vector<std::string>& faceMaterials)
{
    meshSaveFaceData(static_cast<const FaceMesh<V,L,F>&>(mesh), faces, vertexNormals, vertexUV, faceVertexNormals, faceVertexUV, materials, faceMaterials);
}

}
