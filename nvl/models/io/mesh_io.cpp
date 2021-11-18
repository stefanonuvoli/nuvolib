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
            typename Mesh::PolylineColor,
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
        meshLoadVertexData(mesh, data.vertices, data.vertexNormals, data.vertexUVs, data.vertexColors);
        meshLoadPolylineData(mesh, data.polylines, data.polylineColors);
        meshLoadFaceData(mesh, data.faces, data.faceNormals, data.materials, data.faceMaterials, data.vertexNormals, data.vertexUVs, data.faceVertexNormals, data.faceVertexUVs);
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
                typename Mesh::PolylineColor,
                typename Mesh::FaceNormal,
                typename Mesh::Material> data;

        meshSaveVertexData(mesh, data.vertices, data.vertexNormals, data.vertexUVs, data.vertexColors);
        meshSavePolylineData(mesh, data.polylines, data.polylineColors);
        meshSaveFaceData(mesh, data.faces, data.faceNormals, data.materials, data.faceMaterials, data.vertexNormals, data.vertexUVs, data.faceVertexNormals, data.faceVertexUVs);

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
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertices);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexUVs);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexColors);
}

template<class V, class P, class N, class U, class C>
void meshLoadVertexData(
        VertexMesh<V>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors)
{
    typedef typename VertexMesh<V>::VertexId VertexId;

    bool enableVertexNormals = !vertices.empty() && vertices.size() == vertexNormals.size();
    bool enableVertexUV = !vertices.empty() && vertices.size() == vertexUVs.size();
    bool enableVertexColors = !vertices.empty() && vertices.size() == vertexColors.size();

    if (enableVertexNormals) {
        mesh.enableVertexNormals();
    }

    if (enableVertexUV) {
        mesh.enableVertexUVs();
    }

    if (enableVertexColors) {
        mesh.enableVertexColors();
    }

    for (Index i = 0; i < vertices.size(); ++i) {
        const P& point = vertices[i];

        VertexId vId = mesh.addVertex(point);
        assert(i == vId);

        if (enableVertexNormals) {
            mesh.setVertexNormal(vId, vertexNormals[i]);
        }

        if (enableVertexUV) {
            mesh.setVertexUV(vId, vertexUVs[i]);
        }

        if (enableVertexColors) {
            mesh.setVertexColor(vId, vertexColors[i]);
        }
    }
}

template<class V, class L, class P, class N, class U, class C>
void meshLoadVertexData(
        PolylineMesh<V,L>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors)
{
    meshLoadVertexData(static_cast<VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUVs, vertexColors);
}

template<class V, class L, class F, class P, class N, class U, class C>
void meshLoadVertexData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<P>& vertices,
        const std::vector<N>& vertexNormals,
        const std::vector<U>& vertexUVs,
        const std::vector<C>& vertexColors)
{
    meshLoadVertexData(static_cast<VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUVs, vertexColors);
}

template<class Mesh, class C>
void meshLoadPolylineData(
        Mesh& mesh,
        const std::vector<std::vector<Index>>& polylines,
        const std::vector<C>& polylineColors)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(polylines);
    NVL_SUPPRESS_UNUSEDVARIABLE(polylineColors);
}

template<class V, class L, class C>
void meshLoadPolylineData(
        PolylineMesh<V,L>& mesh,
        const std::vector<std::vector<Index>>& polylines,
        const std::vector<C>& polylineColors)
{
    typedef typename PolylineMesh<V,L>::PolylineId PolylineId;

    bool enablePolylineColors = !polylines.empty() && polylines.size() == polylineColors.size();

    if (enablePolylineColors) {
        mesh.enablePolylineColors();
    }

    for (Index i = 0; i < polylines.size(); ++i) {
        const std::vector<Index>& vertexIds = polylines[i];

        PolylineId pId = mesh.addPolyline(vertexIds);

        if (enablePolylineColors) {
            mesh.setPolylineColor(pId, polylineColors[i]);
        }
    }
}

template<class V, class L, class F, class C>
void meshLoadPolylineData(
        FaceMesh<V,L,F>& mesh,
        const std::vector<std::vector<Index>>& polylines,
        const std::vector<C>& polylineColors)
{
    meshLoadPolylineData(static_cast<PolylineMesh<V,L>&>(mesh), polylines, polylineColors);
}

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
        const std::vector<std::vector<Index>>& faceVertexUVs)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(faces);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexUVs);
    NVL_SUPPRESS_UNUSEDVARIABLE(materials);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceMaterials);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceVertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceVertexUVs);
}

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
        const std::vector<std::vector<Index>>& faceVertexUVs)
{
    typedef typename FaceMesh<V,L,F>::MaterialId MaterialId;
    typedef typename FaceMesh<V,L,F>::FaceId FaceId;
    typedef typename FaceMesh<V,L,F>::VertexId VertexId;
    typedef typename FaceMesh<V,L,F>::VertexNormal VertexNormal;
    typedef typename FaceMesh<V,L,F>::UV UV;

    const Index NO_VALUE = NULL_ID;
    const Index MULTIPLE_VALUES = NULL_ID - 1;

    bool enableFaceNormals = !faces.empty() && faces.size() == faceNormals.size();
    bool enableFaceMaterials = !faces.empty() && faces.size() == faceMaterials.size();
    bool enableVertexNormals = false;
    bool enableVertexUVs = false;
    bool enableWedgeNormals = false;
    bool enableWedgeUVs = false;

    std::vector<Index> vertexToNormal(mesh.nextVertexId(), NO_VALUE);
    std::vector<Index> vertexToUV(mesh.nextVertexId(), NO_VALUE);

    if (!faceVertexNormals.empty()) {
        for (Index i = 0; i < faces.size(); ++i) {
            const std::vector<Index>& vertexIds = faces[i];

            for (Index j = 0; j < faceVertexNormals[i].size(); ++j) {
                const VertexId& vId = vertexIds[j];
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

        for (VertexId vId = 0; vId < mesh.nextVertexId() && (!enableVertexNormals || !enableWedgeNormals); ++vId) {
            if (vertexToNormal[vId] != MULTIPLE_VALUES && vertexToNormal[vId] != NO_VALUE) {
                enableVertexNormals = true;
            }
            else if (vertexToNormal[vId] == MULTIPLE_VALUES) {
                enableWedgeNormals = true;
            }
        }
    }


    if (!faceVertexUVs.empty()) {
        for (Index i = 0; i < faces.size(); ++i) {
            const std::vector<Index>& vertexIds = faces[i];

            for (Index j = 0; j < faceVertexUVs[i].size(); ++j) {
                const VertexId& vId = vertexIds[j];
                const Index& uId = faceVertexUVs[i][j];

                Index& mappedUV = vertexToUV[vId];
                if (mappedUV == NO_VALUE) {
                    mappedUV = uId;
                }
                else if (mappedUV != uId) {
                    mappedUV = MULTIPLE_VALUES;
                }
            }
        }

        for (VertexId vId = 0; vId < mesh.nextVertexId() && (!enableVertexUVs || !enableWedgeUVs); ++vId) {
            if (vertexToUV[vId] != MULTIPLE_VALUES && vertexToUV[vId] != NO_VALUE) {
                enableVertexUVs = true;
            }
            else if (vertexToUV[vId] == MULTIPLE_VALUES) {
                enableWedgeUVs = true;
            }
        }
    }

    if (enableFaceNormals) {
        mesh.enableFaceNormals();
    }

    if (enableVertexNormals) {
        mesh.enableVertexNormals();

        for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
            if (vertexToNormal[vId] != MULTIPLE_VALUES && vertexToNormal[vId] != NO_VALUE) {
                mesh.setVertexNormal(vId, vertexNormals[vertexToNormal[vId]]);
            }
        }
    }

    if (enableVertexUVs) {
        mesh.enableVertexUVs();

        for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
            if (vertexToUV[vId] != MULTIPLE_VALUES && vertexToUV[vId] != NO_VALUE) {
                mesh.setVertexUV(vId, vertexUVs[vertexToUV[vId]]);
            }
        }
    }

    if (enableWedgeNormals) {
        mesh.enableWedgeNormals();
    }

    if (enableWedgeUVs) {
        mesh.enableWedgeUVs();
    }

    for (Index i = 0; i < faces.size(); ++i) {
        const std::vector<Index>& vertexIds = faces[i];

        FaceId fId = mesh.addFace(vertexIds);

        if (enableFaceNormals) {
            mesh.setFaceNormal(fId, faceNormals[i]);
        }

        if (enableWedgeNormals) {
            bool customVertexNormals = false;

            std::vector<Index> wedgeNormalIds(faceVertexNormals[i].size(), nvl::NULL_ID);

            for (Index j = 0; j < faceVertexNormals[i].size(); ++j) {
                const VertexId& vId = vertexIds[j];

                assert(vertexToNormal[vId] != NO_VALUE);

                if (vertexToNormal[vId] == MULTIPLE_VALUES) {
                    const VertexNormal& normal = vertexNormals[faceVertexNormals[i][j]];

                    const Index& newWNId = mesh.addWedgeNormal(normal);
                    wedgeNormalIds[j] = newWNId;

                    customVertexNormals = true;
                }
            }

            if (customVertexNormals) {
                mesh.setFaceWedgeNormals(fId, wedgeNormalIds);
            }
        }        

        if (enableWedgeUVs) {
            bool customVertexUVs = false;

            std::vector<Index> wedgeUVIds(faceVertexUVs[i].size(), nvl::NULL_ID);

            for (Index j = 0; j < faceVertexUVs[i].size(); ++j) {
                const VertexId& vId = vertexIds[j];

                assert(vertexToUV[vId] != NO_VALUE);

                if (vertexToUV[vId] == MULTIPLE_VALUES) {
                    const UV& UV = vertexUVs[faceVertexUVs[i][j]];
                    const Index& newWUId = mesh.addWedgeUV(UV);
                    wedgeUVIds[j] = newWUId;

                    customVertexUVs = true;
                }
            }

            if (customVertexUVs) {
                mesh.setFaceWedgeUVs(fId, wedgeUVIds);
            }
        }
    }

    std::map<std::string, MaterialId> materialMap;

    for (const M& material : materials) {
         MaterialId mId = mesh.addMaterial(material);
         materialMap.insert(std::make_pair(material.name(), mId));
    }

    if (enableFaceMaterials) {
        mesh.enableFaceMaterials();

        for (Index i = 0; i < faces.size(); ++i) {
            const std::string& name = faceMaterials[i];
            if (!name.empty() && materialMap.find(name) != materialMap.end()) {
                mesh.setfaceMaterial(i, materialMap.at(name));
            }
        }
    }
}

template<class Mesh, class P, class N, class U, class C>
void meshSaveVertexData(
        const Mesh& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertices);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexUVs);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexColors);
}

template<class V, class P, class N, class U, class C>
void meshSaveVertexData(
        const VertexMesh<V>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors)
{
    typedef typename VertexMesh<V>::Vertex Vertex;

    for (const Vertex& v : mesh.vertices()) {
        vertices.push_back(v.point());

        if (mesh.hasVertexNormals()) {
            vertexNormals.push_back(mesh.vertexNormal(v));
        }

        if (mesh.hasVertexUVs()) {
            vertexUVs.push_back(mesh.vertexUV(v));
        }

        if (mesh.hasVertexColors()) {
            vertexColors.push_back(mesh.vertexColor(v));
        }
    }
}

template<class V, class L, class P, class N, class U, class C>
void meshSaveVertexData(
        const PolylineMesh<V,L>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors)
{
    meshSaveVertexData(static_cast<const VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUVs, vertexColors);
}

template<class V, class L, class F, class P, class N, class U, class C>
void meshSaveVertexData(
        const FaceMesh<V,L,F>& mesh,
        std::vector<P>& vertices,
        std::vector<N>& vertexNormals,
        std::vector<U>& vertexUVs,
        std::vector<C>& vertexColors)
{
    meshSaveVertexData(static_cast<const VertexMesh<V>&>(mesh), vertices, vertexNormals, vertexUVs, vertexColors);
}

template<class Mesh, class C>
void meshSavePolylineData(
        const Mesh& mesh,
        std::vector<std::vector<Index>>& polylines,
        std::vector<C>& polylineColors)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(polylines);
    NVL_SUPPRESS_UNUSEDVARIABLE(polylineColors);
}

template<class V, class P, class C>
void meshSavePolylineData(
        const PolylineMesh<V,P>& mesh,
        std::vector<std::vector<Index>>& polylines,
        std::vector<C>& polylineColors)
{
    typedef typename PolylineMesh<V,P>::Polyline Polyline;
    typedef typename PolylineMesh<V,P>::VertexId VertexId;

    for (const Polyline& p : mesh.polylines()) {
        const std::vector<VertexId>& vertexIds = p.vertexIds();

        polylines.push_back(std::vector<Index>(vertexIds.begin(), vertexIds.end()));

        if (mesh.hasPolylineColors()) {
            polylineColors.push_back(mesh.polylineColor(p));
        }
    }
}

template<class V, class L, class F, class C>
void meshSavePolylineData(
        const FaceMesh<V,L,F>& mesh,
        std::vector<std::vector<Index>>& polylines,
        std::vector<C>& polylineColors)
{
    meshSavePolylineData(static_cast<const PolylineMesh<V,L>&>(mesh), polylines, polylineColors);
}


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
        std::vector<std::vector<Index>>& faceVertexUVs)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mesh);
    NVL_SUPPRESS_UNUSEDVARIABLE(faces);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(vertexUVs);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceVertexNormals);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceVertexUVs);
    NVL_SUPPRESS_UNUSEDVARIABLE(materials);
    NVL_SUPPRESS_UNUSEDVARIABLE(faceMaterials);
}

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
        std::vector<std::vector<Index>>& faceVertexUVs)
{
    typedef typename FaceMesh<V,P,F>::Face Face;
    typedef typename FaceMesh<V,P,F>::VertexNormal VertexNormal;
    typedef typename FaceMesh<V,P,F>::UV UV;
    typedef typename FaceMesh<V,P,F>::Material Material;
    typedef typename Face::VertexContainer VertexContainer;

    for (const Face& f : mesh.faces()) {
        const VertexContainer& vertexIds = f.vertexIds();

        faces.push_back(std::vector<Index>(vertexIds.begin(), vertexIds.end()));

        if (mesh.hasFaceNormals()) {
            faceNormals.push_back(mesh.faceNormal(f));
        }

        if (mesh.hasVertexNormals() || mesh.hasWedgeNormals()) {
            std::vector<Index> normalVec(vertexIds.size(), NULL_ID);

            if (mesh.hasVertexNormals()) {
                normalVec = std::vector<Index>(vertexIds.begin(), vertexIds.end());
            }

            if (mesh.hasWedgeNormals() && !mesh.faceWedgeNormalsAreNull(f)) {
                std::vector<Index> wedgeNormals = mesh.faceWedgeNormals(f);

                for (Index j = 0; j < wedgeNormals.size(); j++) {
                    const Index& nId = wedgeNormals[j];

                    if (nId != NULL_ID) {
                        const VertexNormal& normal = mesh.wedgeNormal(nId);

                        vertexNormals.push_back(normal);
                        Index wedgeNormalId = vertexNormals.size() - 1;

                        normalVec[j] = wedgeNormalId;
                    }
                }
            }

            assert(!normalVec.empty());
            faceVertexNormals.push_back(normalVec);
        }

        if (mesh.hasVertexUVs() || mesh.hasWedgeUVs()) {
            std::vector<Index> uvVec(vertexIds.size(), NULL_ID);

            if (mesh.hasVertexUVs()) {
                uvVec = std::vector<Index>(vertexIds.begin(), vertexIds.end());
            }

            if (mesh.hasWedgeUVs() && !mesh.faceWedgeUVsAreNull(f)) {
                std::vector<Index> wedgeUVs = mesh.faceWedgeUVs(f);

                for (Index j = 0; j < wedgeUVs.size(); j++) {
                    const Index& nId = wedgeUVs[j];

                    if (nId != NULL_ID) {
                        const UV& normal = mesh.wedgeUV(nId);

                        vertexUVs.push_back(normal);
                        Index wedgeUVId = vertexUVs.size() - 1;

                        uvVec[j] = wedgeUVId;
                    }
                }
            }

            assert(!uvVec.empty());
            faceVertexUVs.push_back(uvVec);
        }
    }

    for (const Material& material : mesh.materials()) {
         materials.push_back(material);
    }

    if (mesh.hasFaceMaterials()) {
        for (const Face& f : mesh.faces()) {
            if (!mesh.faceMaterialIsNull(f)) {
                std::string name = mesh.material(mesh.faceMaterial(f)).name();
                faceMaterials.push_back(name);
            }
            else {
                faceMaterials.push_back("");
            }
        }
    }
}

}
