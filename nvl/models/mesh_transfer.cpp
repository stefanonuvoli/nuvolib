#include "mesh_transfer.h"

namespace nvl {

template<class Mesh>
void meshTransfer(
        const Mesh& mesh,
        Mesh& targetMesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::PolylineId PolylineId;
    typedef typename Mesh::FaceId FaceId;

    std::vector<VertexId> birthVertex;
    std::vector<PolylineId> birthPolyline;
    std::vector<FaceId> birthFace;

    return meshTransfer(mesh, targetMesh, birthVertex, birthPolyline, birthFace);
}

template<class Mesh>
void meshTransfer(
        const Mesh& mesh,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline,
        std::vector<typename Mesh::FaceId>& birthFace)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::PolylineId PolylineId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::MaterialId MaterialId;
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::Polyline Polyline;
    typedef typename Mesh::Face Face;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);
    birthPolyline.resize(targetMesh.nextPolylineId(), nvl::MAX_INDEX);
    birthFace.resize(targetMesh.nextFaceId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);
    std::vector<PolylineId> polylineMap(mesh.nextPolylineId(), MAX_INDEX);
    std::vector<FaceId> faceMap(mesh.nextFaceId(), MAX_INDEX);

    std::vector<MaterialId> materialMap(mesh.nextMaterialId(), MAX_INDEX);

    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        const Vertex& vertex = mesh.vertex(vId);

        VertexId newVId = targetMesh.addVertex(vertex);
        vertexMap[vId] = newVId;

        assert(birthVertex.size() == newVId);
        birthVertex.push_back(vId);
    }

    for (PolylineId pId = 0; pId < mesh.nextPolylineId(); ++pId) {
        if (mesh.isPolylineDeleted(pId))
            continue;

        const Polyline& polyline = mesh.polyline(pId);

        std::vector<VertexId> vertexIds(polyline.vertexNumber());
        for (Index j = 0; j < polyline.vertexNumber(); ++j) {
            VertexId vId = polyline.vertexId(j);
            vertexIds[j] = vertexMap[vId];
            assert(vertexIds[j] < MAX_INDEX);
        }

        PolylineId newPId = targetMesh.addPolyline(polyline);
        polylineMap[pId] = newPId;

        targetMesh.polyline(newPId).setVertexIds(vertexIds);

        assert(birthPolyline.size() == newPId);
        birthPolyline.push_back(pId);
    }

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        std::vector<VertexId> vertexIds(face.vertexNumber());
        for (Index j = 0; j < face.vertexNumber(); ++j) {
            VertexId vId = face.vertexId(j);
            vertexIds[j] = vertexMap[vId];
            assert(vertexIds[j] < MAX_INDEX);
        }

        FaceId newFId = targetMesh.addFace(face);
        faceMap[fId] = newFId;

        targetMesh.face(newFId).setVertexIds(vertexIds);

        MaterialId mId = face.materialId();
        if (mId != nvl::MAX_INDEX) {
            if (materialMap[mId] == nvl::MAX_INDEX) {
                MaterialId newMId = targetMesh.addMaterial(mesh.material(mId));
                materialMap[mId] = newMId;
            }
            targetMesh.face(newFId).setMaterialId(materialMap[mId]);
        }

        assert(birthFace.size() == newFId);
        birthFace.push_back(fId);
    }
}

template<class Mesh>
void meshTransferFaces(
        const Mesh& mesh,
        Mesh& targetMesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;

    std::vector<VertexId> birthVertex;
    std::vector<FaceId> birthFace;

    return meshTransferFaces(mesh, targetMesh, birthVertex, birthFace);
}

template<class Mesh>
void meshTransferFaces(
        const Mesh& mesh,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::MaterialId MaterialId;
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::Face Face;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);
    birthFace.resize(targetMesh.nextFaceId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);
    std::vector<FaceId> faceMap(mesh.nextFaceId(), MAX_INDEX);

    std::vector<MaterialId> materialMap(mesh.nextMaterialId(), MAX_INDEX);

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        std::vector<VertexId> vertexIds(face.vertexNumber());
        for (Index j = 0; j < face.vertexNumber(); ++j) {
            VertexId vId = face.vertexId(j);

            if (vertexMap[vId] == MAX_INDEX) {
                const Vertex& vertex = mesh.vertex(vId);

                VertexId newVId = targetMesh.addVertex(vertex);
                vertexMap[vId] = newVId;

                assert(birthVertex.size() == newVId);
                birthVertex.push_back(vId);
            }

            vertexIds[j] = vertexMap[vId];
        }

        FaceId newFId = targetMesh.addFace(face);
        faceMap[fId] = newFId;        

        targetMesh.face(newFId).setVertexIds(vertexIds);

        MaterialId mId = face.materialId();
        if (mId != nvl::MAX_INDEX) {
            if (materialMap[mId] == nvl::MAX_INDEX) {
                MaterialId newMId = targetMesh.addMaterial(mesh.material(mId));
                materialMap[mId] = newMId;
            }
            targetMesh.face(newFId).setMaterialId(materialMap[mId]);
        }

        assert(birthFace.size() == newFId);
        birthFace.push_back(fId);
    }
}

template<class Mesh>
void meshTransferFaces(
        const Mesh& mesh,
        const std::vector<typename Mesh::FaceId>& faces,
        Mesh& targetMesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;

    std::vector<VertexId> birthVertex;
    std::vector<FaceId> birthFace;

    return meshTransferFaces(mesh, faces, targetMesh, birthVertex, birthFace);
}

template<class Mesh>
void meshTransferFaces(
        const Mesh& mesh,
        const std::vector<typename Mesh::FaceId>& faces,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::MaterialId MaterialId;
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::Face Face;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);
    birthFace.resize(targetMesh.nextFaceId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);
    std::vector<FaceId> faceMap(mesh.nextFaceId(), MAX_INDEX);

    std::vector<MaterialId> materialMap(mesh.nextMaterialId(), MAX_INDEX);

    for (FaceId i = 0; i < faces.size(); ++i) {
        FaceId fId = faces[i];

        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        std::vector<VertexId> vertexIds(face.vertexNumber());

        for (Index j = 0; j < face.vertexNumber(); ++j) {
            VertexId vId = face.vertexId(j);

            if (vertexMap[vId] == MAX_INDEX) {
                const Vertex& vertex = mesh.vertex(vId);

                VertexId newVId = targetMesh.addVertex(vertex);
                vertexMap[vId] = newVId;

                assert(birthVertex.size() == newVId);
                birthVertex.push_back(vId);
            }

            vertexIds[j] = vertexMap[vId];
        }

        FaceId newFId = targetMesh.addFace(face);
        faceMap[fId] = newFId;        

        targetMesh.face(newFId).setVertexIds(vertexIds);

        MaterialId mId = face.materialId();
        if (mId != nvl::MAX_INDEX) {
            if (materialMap[mId] == nvl::MAX_INDEX) {
                MaterialId newMId = targetMesh.addMaterial(mesh.material(mId));
                materialMap[mId] = newMId;
            }
            targetMesh.face(newFId).setMaterialId(materialMap[mId]);
        }

        assert(birthFace.size() == newFId);
        birthFace.push_back(fId);
    }
}

template<class Mesh>
void meshTransferPolylines(
        const Mesh& mesh,
        Mesh& targetMesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::PolylineId PolylineId;

    std::vector<VertexId> birthVertex;
    std::vector<PolylineId> birthPolyline;

    return meshTransferPolylines(mesh, targetMesh, birthVertex, birthPolyline);
}

template<class Mesh>
void meshTransferPolylines(
        const Mesh& mesh,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::PolylineId PolylineId;
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::Polyline Polyline;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);
    birthPolyline.resize(targetMesh.nextPolylineId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);
    std::vector<PolylineId> polylineMap(mesh.nextPolylineId(), MAX_INDEX);

    for (PolylineId pId = 0; pId < mesh.nextPolylineId(); ++pId) {
        if (mesh.isPolylineDeleted(pId))
            continue;

        const Polyline& polyline = mesh.polyline(pId);

        std::vector<VertexId> vertexIds(polyline.vertexNumber());
        for (Index j = 0; j < polyline.vertexNumber(); ++j) {
            VertexId vId = polyline.vertexId(j);

            if (vertexMap[vId] == MAX_INDEX) {
                const Vertex& vertex = mesh.vertex(vId);

                VertexId newVId = targetMesh.addVertex(vertex);
                vertexMap[vId] = newVId;

                assert(birthVertex.size() == newVId);
                birthVertex.push_back(vId);
            }

            vertexIds[j] = vertexMap[vId];
        }

        PolylineId newPId = targetMesh.addPolyline(polyline);
        polylineMap[pId] = newPId;        

        targetMesh.polyline(newPId).setVertexIds(vertexIds);

        assert(birthPolyline.size() == newPId);
        birthPolyline.push_back(pId);
    }
}

template<class Mesh>
void meshTransferPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::PolylineId>& polylines,
        Mesh& targetMesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::PolylineId PolylineId;

    std::vector<VertexId> birthVertex;
    std::vector<PolylineId> birthPolyline;

    return meshTransferPolylines(mesh, polylines, targetMesh, birthVertex, birthPolyline);
}

template<class Mesh>
void meshTransferPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::PolylineId>& polylines,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::PolylineId PolylineId;
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::Polyline Polyline;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);
    birthPolyline.resize(targetMesh.nextPolylineId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);
    std::vector<PolylineId> polylineMap(mesh.nextPolylineId(), MAX_INDEX);

    for (PolylineId i = 0; i < polylines.size(); ++i) {
        PolylineId pId = polylines[i];

        if (mesh.isPolylineDeleted(pId))
            continue;

        const Polyline& polyline = mesh.polyline(pId);

        std::vector<VertexId> vertexIds(polyline.vertexNumber());
        for (Index j = 0; j < polyline.vertexNumber(); ++j) {
            VertexId vId = polyline.vertexId(j);

            if (vertexMap[vId] == MAX_INDEX) {
                const Vertex& vertex = mesh.vertex(vId);

                VertexId newVId = targetMesh.addVertex(vertex);
                vertexMap[vId] = newVId;

                assert(birthVertex.size() == newVId);
                birthVertex.push_back(vId);
            }

            vertexIds[j] = vertexMap[vId];
        }

        PolylineId newPId = targetMesh.addPolyline(polyline);
        polylineMap[pId] = newPId;

        targetMesh.polyline(newPId).setVertexIds(vertexIds);

        assert(birthPolyline.size() == newPId);
        birthPolyline.push_back(pId);
    }
}

template<class Mesh>
void meshTransferVertices(
        const Mesh& mesh,
        Mesh& targetMesh)
{
    typedef typename Mesh::VertexId VertexId;

    std::vector<VertexId> birthVertex;

    return meshTransferVertices(mesh, targetMesh, birthVertex);
}

template<class Mesh>
void meshTransferVertices(
        const Mesh& mesh,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Vertex Vertex;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);

    for (VertexId vId = 0; vId < mesh.nextVertexId(); ++vId) {
        if (mesh.isVertexDeleted(vId))
            continue;

        const Vertex& vertex = mesh.vertex(vId);

        VertexId newVId = targetMesh.addVertex(vertex);
        vertexMap[vId] = newVId;

        assert(birthVertex.size() == newVId);
        birthVertex.push_back(vId);
    }
}

template<class Mesh>
void meshTransferVertices(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh)
{
    typedef typename Mesh::VertexId VertexId;

    std::vector<VertexId> birthVertex;

    return meshTransferVertices(mesh, vertices, targetMesh, birthVertex);
}

template<class Mesh>
void meshTransferVertices(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Vertex Vertex;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);

    for (VertexId i = 0; i < vertices.size(); ++i) {
        VertexId vId = vertices[i];

        if (mesh.isVertexDeleted(vId))
            continue;

        const Vertex& vertex = mesh.vertex(vId);

        VertexId newVId = targetMesh.addVertex(vertex);
        vertexMap[vId] = newVId;

        assert(birthVertex.size() == newVId);
        birthVertex.push_back(vId);
    }
}

template<class Mesh>
void meshTransferVerticesWithPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::PolylineId PolylineId;

    std::vector<VertexId> birthVertex;
    std::vector<PolylineId> birthPolyline;

    return meshTransferVerticesWithPolylines(mesh, vertices, targetMesh, birthVertex, birthPolyline);
}

template<class Mesh>
void meshTransferVerticesWithPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::PolylineId PolylineId;
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::Polyline Polyline;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);
    birthPolyline.resize(targetMesh.nextPolylineId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);
    std::vector<PolylineId> polylineMap(mesh.nextPolylineId(), MAX_INDEX);

    for (VertexId i = 0; i < vertices.size(); ++i) {
        VertexId vId = vertices[i];

        if (mesh.isVertexDeleted(vId))
            continue;

        const Vertex& vertex = mesh.vertex(vId);

        VertexId newVId = targetMesh.addVertex(vertex);
        vertexMap[vId] = newVId;

        assert(birthVertex.size() == newVId);
        birthVertex.push_back(vId);
    }

    for (PolylineId pId = 0; pId < mesh.nextPolylineId(); ++pId) {
        if (mesh.isPolylineDeleted(pId))
            continue;

        const Polyline& polyline = mesh.polyline(pId);

        bool hasAllVertex = true;
        std::vector<VertexId> vertexIds(polyline.vertexNumber());
        for (Index j = 0; j < polyline.vertexNumber() && hasAllVertex; ++j) {
            VertexId vId = polyline.vertexId(j);

            if (vertexMap[vId] == MAX_INDEX) {
                hasAllVertex = false;
            }
            else {
                vertexIds[j] = vertexMap[vId];
            }
        }

        if (hasAllVertex) {
            PolylineId newPId = targetMesh.addPolyline(polyline);
            polylineMap[pId] = newPId;

            targetMesh.polyline(newPId).setVertexIds(vertexIds);

            assert(birthPolyline.size() == newPId);
            birthPolyline.push_back(pId);
        }
    }

}

template<class Mesh>
void meshTransferVerticesWithFaces(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;

    std::vector<VertexId> birthVertex;
    std::vector<FaceId> birthFace;

    return meshTransferVerticesWithFaces(mesh, vertices, targetMesh, birthVertex, birthFace);
}

template<class Mesh>
void meshTransferVerticesWithFaces(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::FaceId>& birthFace)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::MaterialId MaterialId;
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::Face Face;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);
    birthFace.resize(targetMesh.nextFaceId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);
    std::vector<FaceId> faceMap(mesh.nextFaceId(), MAX_INDEX);

    std::vector<MaterialId> materialMap(mesh.nextMaterialId(), MAX_INDEX);

    for (VertexId i = 0; i < vertices.size(); ++i) {
        VertexId vId = vertices[i];

        if (mesh.isVertexDeleted(vId))
            continue;

        const Vertex& vertex = mesh.vertex(vId);

        VertexId newVId = targetMesh.addVertex(vertex);
        vertexMap[vId] = newVId;

        assert(birthVertex.size() == newVId);
        birthVertex.push_back(vId);
    }

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {

        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        bool hasAllVertex = true;
        std::vector<VertexId> vertexIds(face.vertexNumber());
        for (Index j = 0; j < face.vertexNumber() && hasAllVertex; ++j) {
            VertexId vId = face.vertexId(j);

            if (vertexMap[vId] == MAX_INDEX) {
                hasAllVertex = false;
            }
            else {
                vertexIds[j] = vertexMap[vId];
            }
        }

        if (hasAllVertex) {
            FaceId newFId = targetMesh.addFace(face);
            faceMap[fId] = newFId;

            targetMesh.face(newFId).setVertexIds(vertexIds);

            MaterialId mId = face.materialId();
            if (mId != nvl::MAX_INDEX) {
                if (materialMap[mId] == nvl::MAX_INDEX) {
                    MaterialId newMId = targetMesh.addMaterial(mesh.material(mId));
                    materialMap[mId] = newMId;
                }
                targetMesh.face(newFId).setMaterialId(materialMap[mId]);
            }

            assert(birthFace.size() == newFId);
            birthFace.push_back(fId);
        }
    }
}

template<class Mesh>
void meshTransferVerticesWithFacesAndPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh)
{

    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::PolylineId PolylineId;

    std::vector<VertexId> birthVertex;
    std::vector<FaceId> birthFace;
    std::vector<PolylineId> birthPolyline;

    return meshTransferVerticesWithFacesAndPolylines(mesh, vertices, targetMesh, birthVertex, birthPolyline, birthFace);
}

template<class Mesh>
void meshTransferVerticesWithFacesAndPolylines(
        const Mesh& mesh,
        const std::vector<typename Mesh::VertexId>& vertices,
        Mesh& targetMesh,
        std::vector<typename Mesh::VertexId>& birthVertex,
        std::vector<typename Mesh::PolylineId>& birthPolyline,
        std::vector<typename Mesh::FaceId>& birthFace)
{
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::PolylineId PolylineId;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::MaterialId MaterialId;
    typedef typename Mesh::Vertex Vertex;
    typedef typename Mesh::Polyline Polyline;
    typedef typename Mesh::Face Face;

    birthVertex.resize(targetMesh.nextVertexId(), nvl::MAX_INDEX);
    birthPolyline.resize(targetMesh.nextPolylineId(), nvl::MAX_INDEX);
    birthFace.resize(targetMesh.nextFaceId(), nvl::MAX_INDEX);

    std::vector<VertexId> vertexMap(mesh.nextVertexId(), MAX_INDEX);
    std::vector<PolylineId> polylineMap(mesh.nextPolylineId(), MAX_INDEX);
    std::vector<FaceId> faceMap(mesh.nextFaceId(), MAX_INDEX);

    std::vector<MaterialId> materialMap(mesh.nextMaterialId(), MAX_INDEX);

    for (VertexId i = 0; i < vertices.size(); ++i) {
        VertexId vId = vertices[i];

        if (mesh.isVertexDeleted(vId))
            continue;

        const Vertex& vertex = mesh.vertex(vId);

        VertexId newVId = targetMesh.addVertex(vertex);
        vertexMap[vId] = newVId;

        assert(birthVertex.size() == newVId);
        birthVertex.push_back(vId);
    }

    for (PolylineId pId = 0; pId < mesh.nextPolylineId(); ++pId) {
        if (mesh.isPolylineDeleted(pId))
            continue;

        const Polyline& polyline = mesh.polyline(pId);

        bool hasAllVertex = true;
        std::vector<VertexId> vertexIds(polyline.vertexNumber());
        for (Index j = 0; j < polyline.vertexNumber() && hasAllVertex; ++j) {
            VertexId vId = polyline.vertexId(j);

            if (vertexMap[vId] == MAX_INDEX) {
                hasAllVertex = false;
            }
            else {
                vertexIds[j] = vertexMap[vId];
            }
        }

        if (hasAllVertex) {
            PolylineId newPId = targetMesh.addPolyline(polyline);
            polylineMap[pId] = newPId;

            targetMesh.polyline(newPId).setVertexIds(vertexIds);

            assert(birthPolyline.size() == newPId);
            birthPolyline.push_back(pId);
        }
    }

    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {

        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        bool hasAllVertex = true;
        std::vector<VertexId> vertexIds(face.vertexNumber());
        for (Index j = 0; j < face.vertexNumber() && hasAllVertex; ++j) {
            VertexId vId = face.vertexId(j);

            if (vertexMap[vId] == MAX_INDEX) {
                hasAllVertex = false;
            }
            else {
                vertexIds[j] = vertexMap[vId];
            }
        }

        if (hasAllVertex) {
            FaceId newFId = targetMesh.addFace(face);
            faceMap[fId] = newFId;

            targetMesh.face(newFId).setVertexIds(vertexIds);

            MaterialId mId = face.materialId();
            if (mId != nvl::MAX_INDEX) {
                if (materialMap[mId] == nvl::MAX_INDEX) {
                    MaterialId newMId = targetMesh.addMaterial(mesh.material(mId));
                    materialMap[mId] = newMId;
                }
                targetMesh.face(newFId).setMaterialId(materialMap[mId]);
            }

            assert(birthFace.size() == newFId);
            birthFace.push_back(fId);
        }
    }
}

}
