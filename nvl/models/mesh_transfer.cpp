#include "mesh_transfer.h"

namespace nvl {

template<class M1, class M2>
void meshTransfer(
        const M1& mesh,
        M2& targetMesh)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::PolylineId PolylineId;
    typedef typename M1::FaceId FaceId;

    std::vector<VertexId> birthVertex;
    std::vector<PolylineId> birthPolyline;
    std::vector<FaceId> birthFace;

    meshTransfer(mesh, targetMesh, birthVertex, birthPolyline, birthFace);
}

template<class M1, class M2>
void meshTransfer(
        const M1& mesh,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline,
        std::vector<typename M1::FaceId>& birthFace)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> vertexMap;

    meshTransferVertices(mesh, targetMesh, birthVertex, vertexMap);
    meshTransferPolylines(mesh, targetMesh, birthPolyline, vertexMap);
    meshTransferFaces(mesh, targetMesh, birthFace, vertexMap);
}

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        M2& targetMesh)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> birthVertex;
    std::vector<VertexId> vertexMap;

    meshTransferVertices(mesh, targetMesh, birthVertex, vertexMap);
}

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> vertices(mesh.vertexNumber());

    Index i = 0;
    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        vertices[i] = vId;
        i++;
    }

    assert(i == mesh.vertexNumber());

    std::vector<VertexId> vertexMap;

    meshTransferVertices(mesh, targetMesh, birthVertex, vertexMap);
}

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> birthVertex;
    std::vector<VertexId> vertexMap;

    return meshTransferVertices(mesh, vertices, targetMesh, birthVertex, vertexMap);
}

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> vertexMap;

    meshTransferVertices(mesh, vertices, targetMesh, birthVertex, vertexMap);
}

template<class M1, class M2>
void meshTransferVertices(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::VertexId>& vertexMap)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::Vertex Vertex;

    if (mesh.hasVertexNormals()) {
        targetMesh.enableVertexNormals();
    }
    if (mesh.hasVertexUVs()) {
        targetMesh.enableVertexUVs();
    }
    if (mesh.hasVertexColors()) {
        targetMesh.enableVertexColors();
    }

    VertexId newVId = targetMesh.nextVertexId();

    birthVertex.resize(newVId + vertices.size(), NULL_ID);
    vertexMap.resize(mesh.nextVertexId(), NULL_ID);

    targetMesh.allocateVertices(vertices.size());
    for (VertexId i = 0; i < vertices.size(); ++i) {
        VertexId vId = vertices[i];
        assert(!mesh.isVertexDeleted(vId));

        const Vertex& vertex = mesh.vertex(vId);
        Vertex& targetVertex = targetMesh.vertex(newVId);

        targetVertex = vertex;
        targetVertex.setId(newVId);

        vertexMap[vId] = newVId;
        birthVertex[newVId] = vId;

        if (mesh.hasVertexNormals()) {
            targetMesh.setVertexNormal(newVId, mesh.vertexNormal(vId));
        }
        if (mesh.hasVertexUVs()) {
            targetMesh.setVertexUV(newVId, mesh.vertexUV(vId));
        }
        if (mesh.hasVertexColors()) {
            targetMesh.setVertexColor(newVId, mesh.vertexColor(vId));
        }

        newVId++;
    }
}

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        M2& targetMesh)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::PolylineId PolylineId;

    std::vector<VertexId> birthVertex;
    std::vector<PolylineId> birthPolyline;

    return meshTransferPolylines(mesh, targetMesh, birthVertex, birthPolyline);
}

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline)
{
    typedef typename M1::PolylineId PolylineId;
    typedef typename M1::VertexId VertexId;

    std::vector<PolylineId> polylines(mesh.polylineNumber());

    Index i = 0;
    for (PolylineId pId = 0; pId < mesh.nextPolylineId(); ++pId) {
        if (mesh.isPolylineDeleted(pId))
            continue;

        polylines[i] = pId;
        i++;
    }

    assert(i == mesh.polylineNumber());

    std::vector<VertexId> vertexMap;

    meshTransferPolylines(mesh, polylines, targetMesh, birthVertex, birthPolyline, vertexMap);
}

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        const std::vector<typename M1::PolylineId>& polylines,
        M2& targetMesh)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::PolylineId PolylineId;

    std::vector<VertexId> birthVertex;
    std::vector<PolylineId> birthPolyline;
    std::vector<VertexId> vertexMap;

    meshTransferPolylines(mesh, polylines, targetMesh, birthVertex, birthPolyline, vertexMap);
}

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        const std::vector<typename M1::PolylineId>& polylines,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> vertexMap;

    meshTransferPolylines(mesh, polylines, targetMesh, birthVertex, birthPolyline, vertexMap);
}

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        const std::vector<typename M1::PolylineId>& polylines,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline,
        std::vector<typename M1::VertexId>& vertexMap)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::PolylineId PolylineId;
    typedef typename M1::Vertex Vertex;
    typedef typename M1::Polyline Polyline;

    if (mesh.hasVertexNormals()) {
        targetMesh.enableVertexNormals();
    }
    if (mesh.hasVertexUVs()) {
        targetMesh.enableVertexUVs();
    }
    if (mesh.hasVertexColors()) {
        targetMesh.enableVertexColors();
    }

    if (mesh.hasPolylineColors()) {
        targetMesh.enablePolylineColors();
    }

    PolylineId newPId = targetMesh.nextPolylineId();

    birthVertex.resize(targetMesh.nextVertexId(), NULL_ID);
    birthPolyline.resize(newPId + polylines.size(), NULL_ID);

    vertexMap.resize(mesh.nextVertexId(), NULL_ID);

    targetMesh.allocatePolylines(polylines.size());
    for (PolylineId i = 0; i < polylines.size(); ++i) {
        PolylineId pId = polylines[i];
        assert(!mesh.isPolylineDeleted(pId));

        const Polyline& polyline = mesh.polyline(pId);
        Polyline& targetPolyline = targetMesh.polyline(newPId);

        targetPolyline = polyline;
        targetPolyline.setId(newPId);

        std::vector<VertexId>& targetVIds = targetPolyline.vertexIds();
        targetVIds.resize(polyline.vertexNumber());
        for (Index j = 0; j < polyline.vertexNumber(); ++j) {
            const VertexId& vId = polyline.vertexId(j);

            if (vertexMap[vId] == NULL_ID) {
                const Vertex& vertex = mesh.vertex(vId);

                VertexId newVId = targetMesh.addVertex(vertex);
                vertexMap[vId] = newVId;

                assert(birthVertex.size() == newVId);
                birthVertex.push_back(vId);

                if (mesh.hasVertexNormals()) {
                    targetMesh.setVertexNormal(newVId, mesh.vertexNormal(vId));
                }
                if (mesh.hasVertexUVs()) {
                    targetMesh.setVertexUV(newVId, mesh.vertexUV(vId));
                }
                if (mesh.hasVertexColors()) {
                    targetMesh.setVertexColor(newVId, mesh.vertexColor(vId));
                }
            }

            targetVIds[j] = vertexMap[vId];
            assert(targetVIds[j] != NULL_ID);

            if (mesh.hasPolylineColors()) {
                targetMesh.setPolylineColor(newPId, mesh.polylineColor(pId));
            }
        }

        birthPolyline[newPId] = pId;

        newPId++;
    }
}

template<class M1, class M2>
void meshTransferPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertexMap,
        M2& targetMesh,
        std::vector<typename M1::PolylineId>& birthPolyline)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::PolylineId PolylineId;
    typedef typename M1::Polyline Polyline;

    if (mesh.hasPolylineColors()) {
        targetMesh.enablePolylineColors();
    }

    birthPolyline.resize(targetMesh.nextPolylineId(), nvl::NULL_ID);
    for (PolylineId pId = 0; pId < mesh.nextPolylineId(); ++pId) {
        if (mesh.isPolylineDeleted(pId))
            continue;

        const Polyline& polyline = mesh.polyline(pId);

        bool hasAllVertex = true;

        std::vector<VertexId> targetVIds(polyline.vertexNumber());
        for (Index j = 0; j < polyline.vertexNumber() && hasAllVertex; ++j) {
            const VertexId& vId = polyline.vertexId(j);

            if (vertexMap[vId] == NULL_ID) {
                hasAllVertex = false;
            }
            else {
                targetVIds[j] = vertexMap[vId];
                assert(targetVIds[j] != NULL_ID);
            }
        }

        if (hasAllVertex) {
            PolylineId newPId = targetMesh.addPolyline(polyline);
            targetMesh.setPolylineVertexIds(newPId, targetVIds);

            assert(birthPolyline.size() == newPId);
            birthPolyline.push_back(pId);

            if (mesh.hasPolylineColors()) {
                targetMesh.setPolylineColor(newPId, mesh.polylineColor(pId));
            }
        }
    }
}

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        M2& targetMesh)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::FaceId FaceId;

    std::vector<VertexId> birthVertex;
    std::vector<FaceId> birthFace;

    return meshTransferFaces(mesh, targetMesh, birthVertex, birthFace);
}

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::FaceId>& birthFace)
{
    typedef typename M1::FaceId FaceId;
    typedef typename M1::VertexId VertexId;

    std::vector<FaceId> faces(mesh.faceNumber());

    Index i = 0;
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        faces[i] = fId;
        i++;
    }

    assert(i == mesh.faceNumber());

    std::vector<VertexId> vertexMap;

    meshTransferFaces(mesh, faces, targetMesh, birthVertex, birthFace, vertexMap);
}

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        const std::vector<typename M1::FaceId>& faces,
        M2& targetMesh)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::FaceId FaceId;

    std::vector<VertexId> birthVertex;
    std::vector<FaceId> birthFace;
    std::vector<VertexId> vertexMap;

    meshTransferFaces(mesh, faces, targetMesh, birthVertex, birthFace, vertexMap);
}

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        const std::vector<typename M1::FaceId>& faces,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::FaceId>& birthFace)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> vertexMap;

    meshTransferFaces(mesh, faces, targetMesh, birthVertex, birthFace, vertexMap);
}


template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        const std::vector<typename M1::FaceId>& faces,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::FaceId>& birthFace,
        std::vector<typename M1::VertexId>& vertexMap)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::FaceId FaceId;
    typedef typename M1::MaterialId MaterialId;
    typedef typename M1::Vertex Vertex;
    typedef typename M1::Face Face;
    typedef typename M1::WedgeNormalId WedgeNormalId;
    typedef typename M1::WedgeUVId WedgeUVId;

    std::vector<MaterialId> materialMap;
    std::vector<WedgeNormalId> wedgeNormalMap;
    std::vector<WedgeUVId> wedgeUVMap;

    if (mesh.hasVertexNormals()) {
        targetMesh.enableVertexNormals();
    }
    if (mesh.hasVertexUVs()) {
        targetMesh.enableVertexUVs();
    }
    if (mesh.hasVertexColors()) {
        targetMesh.enableVertexColors();
    }

    if (mesh.hasFaceMaterials()) {
        targetMesh.enableFaceMaterials();
        materialMap.resize(mesh.nextMaterialId(), NULL_ID);
    }
    if (mesh.hasFaceNormals()) {
        targetMesh.enableFaceNormals();
    }

    if (mesh.hasWedgeNormals()) {
        targetMesh.enableWedgeNormals();
        wedgeNormalMap.resize(mesh.nextWedgeNormalId(), NULL_ID);
    }
    if (mesh.hasWedgeUVs()) {
        targetMesh.enableWedgeUVs();
        wedgeUVMap.resize(mesh.nextWedgeUVId(), NULL_ID);
    }

    FaceId newFId = targetMesh.nextFaceId();

    birthVertex.resize(targetMesh.nextVertexId(), NULL_ID);
    birthFace.resize(newFId + faces.size(), NULL_ID);

    vertexMap.resize(mesh.nextVertexId(), NULL_ID);

    targetMesh.allocateFaces(faces.size());
    for (FaceId i = 0; i < faces.size(); ++i) {
        FaceId fId = faces[i];
        assert(!mesh.isFaceDeleted(fId));

        const Face& face = mesh.face(fId);
        Face& targetFace = targetMesh.face(newFId);

        targetFace = face;
        targetFace.setId(newFId);

        std::vector<VertexId>& targetVIds = targetFace.vertexIds();
        targetVIds.resize(face.vertexNumber());
        for (Index j = 0; j < face.vertexNumber(); ++j) {
            const VertexId& vId = face.vertexId(j);

            if (vertexMap[vId] == NULL_ID) {
                const Vertex& vertex = mesh.vertex(vId);

                VertexId newVId = targetMesh.addVertex(vertex);
                vertexMap[vId] = newVId;

                assert(birthVertex.size() == newVId);
                birthVertex.push_back(vId);

                if (mesh.hasVertexNormals()) {
                    targetMesh.setVertexNormal(newVId, mesh.vertexNormal(vId));
                }
                if (mesh.hasVertexUVs()) {
                    targetMesh.setVertexUV(newVId, mesh.vertexUV(vId));
                }
                if (mesh.hasVertexColors()) {
                    targetMesh.setVertexColor(newVId, mesh.vertexColor(vId));
                }
            }

            targetVIds[j] = vertexMap[vId];
            assert(targetVIds[j] != NULL_ID);
        }

        birthFace[newFId] = fId;

        if (mesh.hasFaceNormals()) {
            targetMesh.setFaceNormal(newFId, mesh.faceNormal(fId));
        }

        if (mesh.hasFaceMaterials() && !mesh.faceMaterialIsNull(fId)) {
            const MaterialId& mId = mesh.faceMaterialId(fId);

            if (materialMap[mId] == NULL_ID) {
                MaterialId newMId = targetMesh.addMaterial(mesh.material(mId));
                materialMap[mId] = newMId;
            }

            targetMesh.setFaceMaterialId(newFId, materialMap[mId]);
        }

        if (mesh.hasWedgeNormals() && !mesh.faceWedgeNormalsAreNull(fId)) {
            const std::vector<WedgeNormalId>& wedgeNormalIds = mesh.faceWedgeNormals(fId);

            std::vector<WedgeNormalId> newWedgeNormalIds(face.vertexNumber(), nvl::NULL_ID);
            for (Index j = 0; j < wedgeNormalIds.size(); j++) {
                const WedgeNormalId& wedgeNormalId = wedgeNormalIds[j];

                if (wedgeNormalId != nvl::NULL_ID) {
                    if (wedgeNormalMap[wedgeNormalId] == NULL_ID) {
                        WedgeNormalId newWedgeNormalId = targetMesh.addWedgeNormal(mesh.wedgeNormal(wedgeNormalId));
                        wedgeNormalMap[wedgeNormalId] = newWedgeNormalId;
                    }
                    newWedgeNormalIds[j] = wedgeNormalMap[wedgeNormalId];
                }
            }

            targetMesh.setFaceWedgeNormals(newFId, newWedgeNormalIds);
        }

        if (mesh.hasWedgeUVs() && !mesh.faceWedgeUVsAreNull(fId)) {
            std::vector<WedgeUVId> newWedgeUVIds(face.vertexNumber(), nvl::NULL_ID);

            const std::vector<WedgeUVId>& wedgeUVIds = mesh.faceWedgeUVs(fId);
            for (Index j = 0; j < wedgeUVIds.size(); j++) {
                const WedgeUVId& wedgeUVId = wedgeUVIds[j];

                if (wedgeUVId != nvl::NULL_ID) {
                    if (wedgeUVMap[wedgeUVId] == NULL_ID) {
                        WedgeUVId newWedgeUVId = targetMesh.addWedgeUV(mesh.wedgeUV(wedgeUVId));
                        wedgeUVMap[wedgeUVId] = newWedgeUVId;
                    }
                    newWedgeUVIds[j] = wedgeUVMap[wedgeUVId];
                }
            }

            targetMesh.setFaceWedgeUVs(newFId, newWedgeUVIds);
        }

        newFId++;
    }
}

template<class M1, class M2>
void meshTransferFaces(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertexMap,
        M2& targetMesh,
        std::vector<typename M1::FaceId>& birthFace)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::FaceId FaceId;
    typedef typename M1::MaterialId MaterialId;
    typedef typename M1::Face Face;
    typedef typename M1::WedgeNormalId WedgeNormalId;
    typedef typename M1::WedgeUVId WedgeUVId;

    std::vector<MaterialId> materialMap;
    std::vector<WedgeNormalId> wedgeNormalMap;
    std::vector<WedgeUVId> wedgeUVMap;

    if (mesh.hasFaceMaterials()) {
        targetMesh.enableFaceMaterials();
        materialMap.resize(mesh.nextMaterialId(), NULL_ID);
    }
    if (mesh.hasFaceNormals()) {
        targetMesh.enableFaceNormals();
    }

    if (mesh.hasWedgeNormals()) {
        targetMesh.enableWedgeNormals();
        wedgeNormalMap.resize(mesh.nextWedgeNormalId(), NULL_ID);
    }
    if (mesh.hasWedgeUVs()) {
        targetMesh.enableWedgeUVs();
        wedgeUVMap.resize(mesh.nextWedgeUVId(), NULL_ID);
    }

    birthFace.resize(targetMesh.nextFaceId(), nvl::NULL_ID);
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        bool hasAllVertex = true;

        std::vector<VertexId> targetVIds(face.vertexNumber());
        for (Index j = 0; j < face.vertexNumber() && hasAllVertex; ++j) {
            const VertexId& vId = face.vertexId(j);

            if (vertexMap[vId] == NULL_ID) {
                hasAllVertex = false;
            }
            else {
                targetVIds[j] = vertexMap[vId];
                assert(targetVIds[j] != NULL_ID);
            }
        }

        if (hasAllVertex) {
            FaceId newFId = targetMesh.addFace(face);
            targetMesh.setFaceVertexIds(newFId, targetVIds);

            assert(birthFace.size() == newFId);
            birthFace.push_back(fId);

            if (mesh.hasFaceNormals()) {
                targetMesh.setFaceNormal(newFId, mesh.faceNormal(fId));
            }

            if (mesh.hasFaceMaterials() && !mesh.faceMaterialIsNull(fId)) {
                const MaterialId& mId = mesh.faceMaterialId(fId);

                if (materialMap[mId] == NULL_ID) {
                    MaterialId newMId = targetMesh.addMaterial(mesh.material(mId));
                    materialMap[mId] = newMId;
                }

                targetMesh.setFaceMaterialId(newFId, materialMap[mId]);
            }

            if (mesh.hasWedgeNormals() && !mesh.faceWedgeNormalsAreNull(fId)) {
                const std::vector<WedgeNormalId>& wedgeNormalIds = mesh.faceWedgeNormals(fId);

                std::vector<WedgeNormalId> newWedgeNormalIds(face.vertexNumber(), nvl::NULL_ID);
                for (Index j = 0; j < wedgeNormalIds.size(); j++) {
                    const WedgeNormalId& wedgeNormalId = wedgeNormalIds[j];

                    if (wedgeNormalId != nvl::NULL_ID) {
                        if (wedgeNormalMap[wedgeNormalId] == NULL_ID) {
                            WedgeNormalId newWedgeNormalId = targetMesh.addWedgeNormal(mesh.wedgeNormal(wedgeNormalId));
                            wedgeNormalMap[wedgeNormalId] = newWedgeNormalId;
                        }
                        newWedgeNormalIds[j] = wedgeNormalMap[wedgeNormalId];
                    }
                }

                targetMesh.setFaceWedgeNormals(newFId, newWedgeNormalIds);
            }

            if (mesh.hasWedgeUVs() && !mesh.faceWedgeUVsAreNull(fId)) {
                std::vector<WedgeUVId> newWedgeUVIds(face.vertexNumber(), nvl::NULL_ID);

                const std::vector<WedgeUVId>& wedgeUVIds = mesh.faceWedgeUVs(fId);
                for (Index j = 0; j < wedgeUVIds.size(); j++) {
                    const WedgeUVId& wedgeUVId = wedgeUVIds[j];

                    if (wedgeUVId != nvl::NULL_ID) {
                        if (wedgeUVMap[wedgeUVId] == NULL_ID) {
                            WedgeUVId newWedgeUVId = targetMesh.addWedgeUV(mesh.wedgeUV(wedgeUVId));
                            wedgeUVMap[wedgeUVId] = newWedgeUVId;
                        }
                        newWedgeUVIds[j] = wedgeUVMap[wedgeUVId];
                    }
                }

                targetMesh.setFaceWedgeUVs(newFId, newWedgeUVIds);
            }
        }
    }
}

template<class M1, class M2>
void meshTransferVerticesWithPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::PolylineId PolylineId;

    std::vector<VertexId> birthVertex;
    std::vector<PolylineId> birthPolyline;

    return meshTransferVerticesWithPolylines(mesh, vertices, targetMesh, birthVertex, birthPolyline);
}

template<class M1, class M2>
void meshTransferVerticesWithPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> vertexMap;
    meshTransferVertices(mesh, vertices, targetMesh, birthVertex, vertexMap);

    meshTransferPolylines(mesh, vertexMap, targetMesh, birthPolyline);
}

template<class M1, class M2>
void meshTransferVerticesWithFaces(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh)
{
    typedef typename M1::VertexId VertexId;
    typedef typename M1::FaceId FaceId;

    std::vector<VertexId> birthVertex;
    std::vector<FaceId> birthFace;

    return meshTransferVerticesWithFaces(mesh, vertices, targetMesh, birthVertex, birthFace);
}

template<class M1, class M2>
void meshTransferVerticesWithFaces(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::FaceId>& birthFace)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> vertexMap;
    meshTransferVertices(mesh, vertices, targetMesh, birthVertex, vertexMap);

    meshTransferFaces(mesh, vertexMap, targetMesh, birthFace);
}

template<class M1, class M2>
void meshTransferVerticesWithFacesAndPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh)
{

    typedef typename M1::VertexId VertexId;
    typedef typename M1::FaceId FaceId;
    typedef typename M1::PolylineId PolylineId;

    std::vector<VertexId> birthVertex;
    std::vector<FaceId> birthFace;
    std::vector<PolylineId> birthPolyline;

    return meshTransferVerticesWithFacesAndPolylines(mesh, vertices, targetMesh, birthVertex, birthPolyline, birthFace);
}

template<class M1, class M2>
void meshTransferVerticesWithFacesAndPolylines(
        const M1& mesh,
        const std::vector<typename M1::VertexId>& vertices,
        M2& targetMesh,
        std::vector<typename M1::VertexId>& birthVertex,
        std::vector<typename M1::PolylineId>& birthPolyline,
        std::vector<typename M1::FaceId>& birthFace)
{
    typedef typename M1::VertexId VertexId;

    std::vector<VertexId> vertexMap;
    meshTransferVertices(mesh, vertices, targetMesh, birthVertex, vertexMap);

    meshTransferFaces(mesh, vertexMap, targetMesh, birthFace);
    meshTransferPolylines(mesh, vertexMap, targetMesh, birthPolyline);
}

}
