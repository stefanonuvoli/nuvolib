#include "mesh_io.h"

#include <nvl/utilities/file_utils.h>
#include <unordered_map>

namespace nvl {

template<class M>
bool meshLoadFromFile(
        const std::string& filename,
        M& mesh,
        IOMeshError& error,
        const IOMeshMode& mode)
{
    IOMeshData<
            typename M::Point,
            typename M::VertexNormal,
            typename M::VertexUV,
            typename M::VertexColor,
            typename M::PolylineColor,
            typename M::FaceNormal,
            typename M::Material> meshData;

    std::string ext = filenameExtension(filename);

    mesh.clear();

    bool success;
    if (ext == "obj") {
        success = meshLoadDataFromOBJ(filename, meshData, error);
    }
    else {
        error = IO_MESH_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        meshLoadData(mesh, meshData, mode);
    }

    return success;
}

template<class M>
bool meshSaveToFile(
        const std::string& filename,
        const M& mesh,
        IOMeshError& error,
        const IOMeshMode& mode)
{
    std::string ext = filenameExtension(filename);

    bool success;
    if (ext == "obj") {
        IOMeshData<
                typename M::Point,
                typename M::VertexNormal,
                typename M::VertexUV,
                typename M::VertexColor,
                typename M::PolylineColor,
                typename M::FaceNormal,
                typename M::Material> meshData;

        meshSaveData(mesh, meshData, mode);

        success = meshSaveDataToOBJ(filename, meshData, error);
    }
    else {
        error = IO_MESH_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}

template<class VT, class MD>
void meshLoadData(
        VertexMesh<VT>& mesh,
        const MD& meshData,
        const IOMeshMode& mode)
{
    meshLoadVertexData(mesh, meshData, mode);
}

template<class VT, class PT, class MD>
void meshLoadData(
        PolylineMesh<VT,PT> mesh,
        const MD& meshData,
        const IOMeshMode& mode)
{
    meshLoadVertexData(mesh, meshData, mode);
    meshLoadPolylineData(mesh, meshData, mode);
}

template<class VT, class PT, class FT, class MD>
void meshLoadData(
        FaceMesh<VT,PT,FT>& mesh,
        const MD& meshData,
        const IOMeshMode& mode)
{
    meshLoadVertexData(mesh, meshData, mode);
    meshLoadPolylineData(mesh, meshData, mode);
    meshLoadFaceData(mesh, meshData, mode);
}

template<class VT, class MD>
void meshSaveData(
        const VertexMesh<VT>& mesh,
        MD& meshData,
        const IOMeshMode& mode)
{
    meshSaveVertexData(mesh, meshData, mode);
}

template<class VT, class PT, class MD>
void meshSaveData(
        const PolylineMesh<VT,PT> mesh,
        MD& meshData,
        const IOMeshMode& mode)
{
    meshSaveVertexData(mesh, meshData, mode);
    meshSavePolylineData(mesh, meshData, mode);
}

template<class VT, class PT, class FT, class MD>
void meshSaveData(
        const FaceMesh<VT,PT,FT>& mesh,
        MD& meshData,
        const IOMeshMode& mode)
{
    meshSaveVertexData(mesh, meshData, mode);
    meshSavePolylineData(mesh, meshData, mode);
    meshSaveFaceData(mesh, meshData, mode);
}

template<class M, class MD>
void meshLoadVertexData(
        M& mesh,
        const MD& meshData,
        const IOMeshMode& mode)
{
    typedef typename M::VertexId VertexId;
    typedef typename M::Point Point;
    typedef typename M::VertexNormal VertexNormal;
    typedef typename M::VertexUV VertexUV;
    typedef typename M::VertexColor VertexColor;

    if (mode.vertices) {
        const std::vector<Point>& vertices = meshData.vertices;
        const std::vector<VertexNormal>& vertexNormals = meshData.vertexNormals;
        const std::vector<VertexUV>& vertexUVs = meshData.vertexUVs;
        const std::vector<VertexColor>& vertexColors = meshData.vertexColors;

        bool vertexNormalsEnabled = mode.vertexNormals && !vertices.empty() && vertices.size() == vertexNormals.size();
        bool vertexUVsEnabled = mode.vertexUVs && !vertices.empty() && vertices.size() == vertexUVs.size();
        bool vertexColorsEnabled = mode.vertexColors && !vertices.empty() && vertices.size() == vertexColors.size();

        if (vertexNormalsEnabled) {
            mesh.enableVertexNormals();
        }

        if (vertexUVsEnabled) {
            mesh.enableVertexUVs();
        }

        if (vertexColorsEnabled) {
            mesh.enableVertexColors();
        }

        VertexId vId = mesh.allocateVertices(vertices.size());

        for (Index i = 0; i < vertices.size(); ++i) {
            const Point& point = vertices[i];

            mesh.setVertexPoint(vId, point);

            if (vertexNormalsEnabled) {
                mesh.setVertexNormal(vId, vertexNormals[i]);
            }

            if (vertexUVsEnabled) {
                mesh.setVertexUV(vId, vertexUVs[i]);
            }

            if (vertexColorsEnabled) {
                mesh.setVertexColor(vId, vertexColors[i]);
            }

            ++vId;
        }
    }
}

template<class M, class MD>
void meshLoadPolylineData(
        M& mesh,
        const MD& meshData,
        const IOMeshMode& mode)
{
    typedef typename M::PolylineId PolylineId;
    typedef typename M::PolylineColor PolylineColor;

    if (mode.polylines) {
        const std::vector<std::vector<Index>>& polylines = meshData.polylines;
        const std::vector<PolylineColor>& polylineColors = meshData.polylineColors;

        bool polylineColorsEnabled = mode.polylineColors && !polylines.empty() && polylines.size() == polylineColors.size();

        if (polylineColorsEnabled) {
            mesh.enablePolylineColors();
        }

        PolylineId pId = mesh.allocatePolylines(polylines.size());

        for (Index i = 0; i < polylines.size(); ++i) {
            std::vector<Index> vertexIds = polylines[i];

            mesh.setPolylineVertexIds(pId, vertexIds);

            if (polylineColorsEnabled) {
                mesh.setPolylineColor(pId, polylineColors[i]);
            }

            ++pId;
        }
    }
}

template<class M, class MD>
void meshLoadFaceData(
        M& mesh,
        const MD& meshData,
        const IOMeshMode& mode)
{
    typedef typename M::FaceId FaceId;
    typedef typename M::FaceNormal FaceNormal;
    typedef typename M::Material Material;
    typedef typename M::VertexId VertexId;
    typedef typename M::VertexNormal VertexNormal;
    typedef typename M::VertexUV VertexUV;
    if (mode.materials) {
        const std::vector<Material>& materials = meshData.materials;
        for (Index i = 0; i < materials.size(); ++i) {
             mesh.addMaterial(materials[i]);
        }
    }

    if (mode.faces) {
        const std::vector<std::vector<Index>>& faces = meshData.faces;
        const std::vector<FaceNormal>& faceNormals = meshData.faceNormals;
        const std::vector<Index>& faceMaterials = meshData.faceMaterials;
        const std::vector<VertexNormal>& vertexNormals = meshData.vertexNormals;
        const std::vector<VertexUV>& vertexUVs = meshData.vertexUVs;
        const std::vector<std::vector<Index>>& faceVertexNormals = meshData.faceVertexNormals;
        const std::vector<std::vector<Index>>& faceVertexUVs = meshData.faceVertexUVs;

        const Index NO_VALUE = NULL_ID;
        const Index MULTIPLE_VALUES = NULL_ID - 1;

        bool faceNormalsEnabled = mode.faceNormals && !faces.empty() && faces.size() == faceNormals.size();
        bool faceMaterialEnabled = mode.materials && !faces.empty() && faces.size() == faceMaterials.size();
        bool vertexNormalsEnabled = false;
        bool vertexUVsEnabled = false;
        bool wedgeNormalsEnabled = false;
        bool wedgeUVsEnabled = false;

        std::vector<Index> vertexToNormal(mesh.nextVertexId(), NO_VALUE);
        std::vector<Index> vertexToUV(mesh.nextVertexId(), NO_VALUE);

        if (mode.vertexNormals && !faceVertexNormals.empty()) {
            for (Index i = 0; i < faces.size(); ++i) {
                const std::vector<Index>& vertexIds = faces[i];

                for (Index j = 0; j < faceVertexNormals[i].size(); ++j) {
                    const VertexId& vId = vertexIds[j];
                    const Index& nId = faceVertexNormals[i][j];

                    Index& mappedNormalId = vertexToNormal[vId];
                    if (mappedNormalId == NO_VALUE) {
                        mappedNormalId = nId;
                    }
                    else if (mappedNormalId != nId && mappedNormalId != MULTIPLE_VALUES) {
                        const VertexNormal& mappedNormal = vertexNormals[mappedNormalId];
                        const VertexNormal& normal = vertexNormals[nId];

                        if (!nvl::epsEqual(mappedNormal, normal)) {
                            mappedNormalId = MULTIPLE_VALUES;
                        }
                    }
                }
            }

            for (VertexId vId = 0; vId < mesh.nextVertexId() && (!vertexNormalsEnabled || !wedgeNormalsEnabled); ++vId) {
                if (vertexToNormal[vId] != MULTIPLE_VALUES && vertexToNormal[vId] != NO_VALUE) {
                    vertexNormalsEnabled = true;
                }
                else if (vertexToNormal[vId] == MULTIPLE_VALUES) {
                    wedgeNormalsEnabled = true;
                }
            }
        }

        if (mode.vertexUVs && !faceVertexUVs.empty()) {
            for (Index i = 0; i < faces.size(); ++i) {
                const std::vector<Index>& vertexIds = faces[i];

                for (Index j = 0; j < faceVertexUVs[i].size(); ++j) {
                    const VertexId& vId = vertexIds[j];
                    const Index& nId = faceVertexUVs[i][j];

                    Index& mappedUVId = vertexToUV[vId];
                    if (mappedUVId == NO_VALUE) {
                        mappedUVId = nId;
                    }
                    else if (mappedUVId != nId && mappedUVId != MULTIPLE_VALUES) {
                        const VertexUV& mappedUV = vertexUVs[mappedUVId];
                        const VertexUV& uv = vertexUVs[nId];

                        if (!nvl::epsEqual(mappedUV, uv)) {
                            mappedUVId = MULTIPLE_VALUES;
                        }
                    }
                }
            }

            for (VertexId vId = 0; vId < mesh.nextVertexId() && (!vertexUVsEnabled || !wedgeUVsEnabled); ++vId) {
                if (vertexToUV[vId] != MULTIPLE_VALUES && vertexToUV[vId] != NO_VALUE) {
                    vertexUVsEnabled = true;
                }
                else if (vertexToUV[vId] == MULTIPLE_VALUES) {
                    wedgeUVsEnabled = true;
                }
            }
        }


        if (faceMaterialEnabled) {
            mesh.enableFaceMaterials();
        }

        if (faceNormalsEnabled) {
            mesh.enableFaceNormals();
        }

        if (vertexNormalsEnabled) {
            mesh.enableVertexNormals();

            for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
                if (vertexToNormal[vId] != MULTIPLE_VALUES && vertexToNormal[vId] != NO_VALUE) {
                    mesh.setVertexNormal(vId, vertexNormals[vertexToNormal[vId]]);
                }
            }
        }

        if (vertexUVsEnabled) {
            mesh.enableVertexUVs();

            for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
                if (vertexToUV[vId] != MULTIPLE_VALUES && vertexToUV[vId] != NO_VALUE) {
                    mesh.setVertexUV(vId, vertexUVs[vertexToUV[vId]]);
                }
            }
        }

        if (wedgeNormalsEnabled) {
            mesh.enableWedgeNormals();
        }

        if (wedgeUVsEnabled) {
            mesh.enableWedgeUVs();
        }

        FaceId fId = mesh.allocateFaces(faces.size());

        for (Index i = 0; i < faces.size(); ++i) {
            std::vector<Index> vertexIds = faces[i];

            mesh.setFaceVertexIds(fId, vertexIds);

            if (faceNormalsEnabled) {
                mesh.setFaceNormal(fId, faceNormals[i]);
            }

            if (wedgeNormalsEnabled) {
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

            if (wedgeUVsEnabled) {
                bool customVertexUVs = false;

                std::vector<Index> wedgeUVIds(faceVertexUVs[i].size(), nvl::NULL_ID);

                for (Index j = 0; j < faceVertexUVs[i].size(); ++j) {
                    const VertexId& vId = vertexIds[j];

                    assert(vertexToUV[vId] != NO_VALUE);

                    if (vertexToUV[vId] == MULTIPLE_VALUES) {
                        const VertexUV& uv = vertexUVs[faceVertexUVs[i][j]];
                        const Index& newWUId = mesh.addWedgeUV(uv);
                        wedgeUVIds[j] = newWUId;

                        customVertexUVs = true;
                    }
                }

                if (customVertexUVs) {
                    mesh.setFaceWedgeUVs(fId, wedgeUVIds);
                }
            }

            if (faceMaterialEnabled) {
                mesh.setFaceMaterial(fId, faceMaterials[i]);
            }

            ++fId;
        }
    }
}

template<class M, class MD>
void meshSaveVertexData(
        const M& mesh,
        MD& meshData,
        const IOMeshMode& mode)
{
    typedef typename M::Vertex Vertex;
    typedef typename M::Point Point;
    typedef typename M::VertexNormal VertexNormal;
    typedef typename M::VertexUV VertexUV;
    typedef typename M::VertexColor VertexColor;

    if (mode.vertices) {
        std::vector<Point>& vertices = meshData.vertices;
        std::vector<VertexNormal>& vertexNormals = meshData.vertexNormals;
        std::vector<VertexUV>& vertexUVs = meshData.vertexUVs;
        std::vector<VertexColor>& vertexColors = meshData.vertexColors;

        bool vertexNormalsEnabled = mode.vertexNormals && mesh.hasVertexNormals();
        bool vertexUVsEnabled = mode.vertexUVs && mesh.hasVertexUVs();
        bool vertexColorsEnabled = mode.vertexColors && mesh.hasVertexColors();


        vertices.resize(mesh.vertexNumber());

        if (vertexNormalsEnabled) {
            vertexNormals.resize(mesh.vertexNumber());
        }
        if (vertexUVsEnabled) {
            vertexUVs.resize(mesh.vertexNumber());
        }
        if (vertexColorsEnabled) {
            vertexColors.resize(mesh.vertexNumber());
        }

        Index vId = 0;
        for (const Vertex& v : mesh.vertices()) {
            vertices[vId] = v.point();

            if (vertexNormalsEnabled) {
                vertexNormals[vId] = mesh.vertexNormal(v);
            }

            if (vertexUVsEnabled) {
                vertexUVs[vId] = mesh.vertexUV(v);
            }

            if (vertexColorsEnabled) {
                vertexColors[vId] = mesh.vertexColor(v);
            }

            ++vId;
        }
    }
}

template<class M, class MD>
void meshSavePolylineData(
        const M& mesh,
        MD& meshData,
        const IOMeshMode& mode)
{
    typedef typename M::Polyline Polyline;
    typedef typename M::PolylineContainer PolylineContainer;
    typedef typename M::PolylineColor PolylineColor;

    if (mode.polylines) {
        bool polylineColorsEnabled = mode.polylineColors && mesh.hasPolylineColors();

        std::vector<std::vector<Index>>& polylines = meshData.polylines;
        std::vector<PolylineColor>& polylineColors = meshData.polylineColors;

        polylines.resize(mesh.polylineNumber());

        if (polylineColorsEnabled) {
            polylineColors.resize(mesh.polylineNumber());
        }

        Index pId = 0;
        for (const Polyline& p : mesh.polylines()) {
            const PolylineContainer& vertexIds = p.vertexIds();

            polylines.push_back(std::vector<Index>(vertexIds.begin(), vertexIds.end()));

            if (polylineColorsEnabled) {
                polylineColors[pId] = mesh.polylineColor(p);
            }

            ++pId;
        }
    }
}

template<class M, class MD>
void meshSaveFaceData(
        const M& mesh,
        MD& meshData,
        const IOMeshMode& mode)
{
    typedef typename M::Face Face;
    typedef typename M::FaceContainer FaceContainer;
    typedef typename M::FaceNormal FaceNormal;
    typedef typename M::Material Material;
    typedef typename M::VertexNormal VertexNormal;
    typedef typename M::VertexUV VertexUV;

    std::vector<Index> materialMap(mesh.nextMaterialId());

    if (mode.materials) {
        std::vector<Material>& materials = meshData.materials;

        materials.resize(mesh.materialNumber());

        Index mId = 0;
        for (const Material& material : mesh.materials()) {
             materials[mId] = material;

             materialMap[material.id()] = mId;

             ++mId;
        }
    }

    if (mode.faces) {
        std::vector<std::vector<Index>>& faces = meshData.faces;
        std::vector<FaceNormal>& faceNormals = meshData.faceNormals;
        std::vector<Index>& faceMaterials = meshData.faceMaterials;
        std::vector<VertexNormal>& vertexNormals = meshData.vertexNormals;
        std::vector<VertexUV>& vertexUVs = meshData.vertexUVs;
        std::vector<std::vector<Index>>& faceVertexNormals = meshData.faceVertexNormals;
        std::vector<std::vector<Index>>& faceVertexUVs = meshData.faceVertexUVs;

        bool faceNormalsEnabled = mode.faceNormals && mesh.hasFaceNormals();
        bool faceMaterialsEnabled = mode.materials && mesh.hasFaceMaterials();
        bool vertexNormalsEnabled = mode.vertexNormals && (mesh.hasVertexNormals() || mesh.hasWedgeNormals());
        bool vertexUVsEnabled = mode.vertexUVs && (mesh.hasVertexUVs() || mesh.hasWedgeUVs());

        faces.resize(mesh.faceNumber());

        if (faceNormalsEnabled) {
            faceNormals.resize(mesh.faceNumber());
        }
        if (faceMaterialsEnabled) {
            faceMaterials.resize(mesh.faceNumber());
        }
        if (vertexNormalsEnabled) {
            faceVertexNormals.resize(mesh.faceNumber());
        }
        if (vertexUVsEnabled) {
            faceVertexUVs.resize(mesh.faceNumber());
        }

        Index fId = 0;
        for (const Face& f : mesh.faces()) {
            const FaceContainer& vertexIds = f.vertexIds();

            faces[fId] = std::vector<Index>(vertexIds.begin(), vertexIds.end());

            if (faceNormalsEnabled) {
                faceNormals[fId] = mesh.faceNormal(f);
            }

            if (faceMaterialsEnabled) {
                faceMaterials[fId] = materialMap[mesh.faceMaterial(f)];
            }

            if (vertexNormalsEnabled) {
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
                faceVertexNormals[fId] = normalVec;
            }

            if (vertexUVsEnabled) {
                std::vector<Index> uvVec(vertexIds.size(), NULL_ID);

                if (mesh.hasVertexUVs()) {
                    uvVec = std::vector<Index>(vertexIds.begin(), vertexIds.end());
                }

                if (mesh.hasWedgeUVs() && !mesh.faceWedgeUVsAreNull(f)) {
                    std::vector<Index> wedgeUVs = mesh.faceWedgeUVs(f);

                    for (Index j = 0; j < wedgeUVs.size(); j++) {
                        const Index& nId = wedgeUVs[j];

                        if (nId != NULL_ID) {
                            const VertexUV& uv = mesh.wedgeUV(nId);

                            vertexUVs.push_back(uv);
                            Index wedgeUVId = vertexUVs.size() - 1;

                            uvVec[j] = wedgeUVId;
                        }
                    }
                }

                assert(!uvVec.empty());
                faceVertexUVs[fId] = uvVec;
            }

            ++fId;
        }
    }
}

}
