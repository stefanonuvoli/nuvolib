#include "face_mesh_drawer.h"

#include <nvl/viewer/gl/draw_primitives.h>
#include <nvl/viewer/gl/draw_gl.h>

#include <nvl/math/constants.h>
#include <nvl/math/numeric_limits.h>

#include <nvl/models/mesh_geometric_information.h>

#ifdef NVL_STB_LOADED
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/std_image.h>
#undef STB_IMAGE_STATIC
#undef STB_IMAGE_IMPLEMENTATION
#endif

namespace nvl {

template<class M>
FaceMeshDrawer<M>::FaceMeshDrawer() : FaceMeshDrawer<M>(nullptr)
{

}

template<class M>
FaceMeshDrawer<M>::FaceMeshDrawer(M* mesh, const bool visible, const bool pickable) :
    FaceMeshDrawerBase(),
    PolylineMeshDrawer<M>(mesh, visible, pickable)
{
    resetRenderingFaceData();

    this->setVertexVisible(false);
}

template<class M>
FaceMeshDrawer<M>::~FaceMeshDrawer()
{
    clearTextures();
}

template<class M>
void FaceMeshDrawer<M>::draw() const
{
    if (this->vMesh == nullptr)
        return;

    PolylineMeshDrawer<M>::draw();

    if (this->vFaceVisible) {
        if (this->vFaceShadingMode == FaceMeshDrawerBase::FACE_SHADING_SMOOTH) {
            drawFaceSmoothShading();
        }
        else if (this->vFaceShadingMode == FaceMeshDrawerBase::FACE_SHADING_FLAT) {
            drawFaceFlatShading();
        }

        if (this->vFaceShaderMode == FaceMeshDrawerBase::FaceShaderMode::FACE_SHADER_VERTEX_VALUE) {
            drawVertexValueShader();
        }
        else {
            assert(this->vFaceShaderMode == FaceMeshDrawerBase::FaceShaderMode::FACE_SHADER_NONE);
        }
    }

    if (this->vWireframeVisible) {
        drawWireframe();
    }

    if (this->vFaceNormalVisible) {
        drawFaceNormals();
    }

}

template<class M>
void FaceMeshDrawer<M>::drawWithNames(Canvas* canvas, const Index drawableId) const
{
    if (this->vMesh == nullptr || !this->isPickable())
        return;

    PolylineMeshDrawer<M>::drawWithNames(canvas, drawableId);

    if (this->vFaceVisible) {
        if (this->vFaceShadingMode == FaceMeshDrawerBase::FACE_SHADING_SMOOTH) {
            drawFaceSmoothShadingWithNames(canvas, drawableId);
        }
        else if (this->vFaceShadingMode == FaceMeshDrawerBase::FACE_SHADING_FLAT) {
            drawFaceFlatShadingWithNames(canvas, drawableId);
        }
    }

    if (this->vWireframeVisible) {
        drawWireframeWithNames(canvas, drawableId);
    }
}

template<class M>
void FaceMeshDrawer<M>::update()
{
    PolylineMeshDrawer<M>::update();

    resetRenderingFaceData();
}

template<class M>
const std::vector<unsigned int>& FaceMeshDrawer<M>::renderingFace(const Index &id) const
{
    Index mappedId = vFaceMap[id];
    return this->vRenderingFaces[mappedId];
}

template<class M>
Vector3d FaceMeshDrawer<M>::renderingFaceNormal(const Index& id) const
{
    Index mappedId = vFaceMap[id];
    return Vector3d(this->vRenderingFaceNormals[mappedId*3], this->vRenderingFaceNormals[mappedId*3+1], this->vRenderingFaceNormals[mappedId*3+2]);
}

template<class M>
Color FaceMeshDrawer<M>::renderingFaceColor(const Index& id) const
{
    Index mappedId = vFaceMap[id];
    return Color(this->vRenderingFaceColors[mappedId*4], this->vRenderingFaceColors[mappedId*4+1], this->vRenderingFaceColors[mappedId*4+2], this->vRenderingFaceColors[mappedId*4+3]);
}

template<class M>
Color FaceMeshDrawer<M>::renderingFaceWireframeColor(const Index& id) const
{
    Index mappedId = vFaceMap[id];
    return Color(this->vRenderingFaceWireframeColors[mappedId*4], this->vRenderingFaceWireframeColors[mappedId*4+1], this->vRenderingFaceWireframeColors[mappedId*4+2], this->vRenderingFaceWireframeColors[mappedId*4+3]);
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingData()
{
    PolylineMeshDrawer<M>::resetRenderingData();
    resetRenderingFaceData();
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceData()
{
    typedef typename M::Face Face;

    if (this->vMesh == nullptr)
        return;

    vFaceMap.resize(this->vMesh->faceNumber(), MAX_INDEX);

    Index i = 0;
    for (const Face& face : this->vMesh->faces()) {
        vFaceMap[face.id()] = i;
        ++i;
    }

    resetRenderingFaces();
    resetRenderingFaceNormals();
    resetRenderingFaceColors();
    resetRenderingFaceWireframeColors();
    resetRenderingFaceUVs();
    resetRenderingFaceMaterials();

    loadTextures();
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaces()
{
    typedef typename M::FaceId FaceId;

    this->vRenderingFaces.resize(this->vMesh->faceNumber(), std::vector<unsigned int>());

    #pragma omp parallel for
    for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (!this->vMesh->isFaceDeleted(fId)) {
            resetRenderingFace(fId);
        }
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceNormals()
{
    typedef typename M::FaceId FaceId;

    this->vRenderingFaceNormals.resize(this->vMesh->faceNumber() * 3);

    #pragma omp parallel for
    for (FaceId vId = 0; vId < this->vMesh->nextFaceId(); ++vId) {
        if (!this->vMesh->isFaceDeleted(vId)) {
            resetRenderingFaceNormal(vId);
        }
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceColors()
{
    typedef typename M::FaceId FaceId;

    this->vRenderingFaceColors.resize(this->vMesh->faceNumber() * 4);

    #pragma omp parallel for
    for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (!this->vMesh->isFaceDeleted(fId)) {
            resetRenderingFaceColor(fId);
        }
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceUVs()
{
    typedef typename M::FaceId FaceId;

    this->vRenderingFaceUVs.resize(this->vMesh->faceNumber());

    #pragma omp parallel for
    for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (!this->vMesh->isFaceDeleted(fId)) {
            resetRenderingFaceUV(fId);
        }
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceWireframeColors()
{
    typedef typename M::FaceId FaceId;

    this->vRenderingFaceWireframeColors.resize(this->vMesh->faceNumber() * 4);

    #pragma omp parallel for
    for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (!this->vMesh->isFaceDeleted(fId)) {
            resetRenderingFaceWireframeColor(fId);
        }
    }
}


template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceMaterials()
{
    typedef typename M::FaceId FaceId;

    this->vRenderingFaceMaterials.resize(this->vMesh->faceNumber());

    #pragma omp parallel for
    for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (!this->vMesh->isFaceDeleted(fId)) {
            resetRenderingFaceMaterial(fId);
        }
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFace(const Index& id)
{
    typedef typename M::Face Face;

    const Face& face = this->vMesh->face(id);

    std::vector<unsigned int> vertexIndices(face.vertexNumber());

    #pragma omp parallel for
    for (Index j = 0; j < face.vertexNumber(); ++j) {
        vertexIndices[j] = this->vVertexMap[face.vertexId(j)];
    }

    setRenderingFace(id, vertexIndices);
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceNormal(const Index& id)
{
    typedef typename M::Face Face;
    const Face& face = this->vMesh->face(id);
    setRenderingFaceNormal(id, face.normal());
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceColor(const Index& id)
{
    typedef typename M::Face Face;
    typedef typename M::Material Material;
    typedef typename M::MaterialColor MaterialColor;

    const Face& face = this->vMesh->face(id);

    MaterialColor c;
    if (this->vMesh->hasFaceMaterial(face)) {
        const Material& material = this->vMesh->faceMaterial(face);
        c = material.diffuseColor();
        c.setAlphaF(1.0 - material.transparency());
    }
    else {
        c = Color(0.7, 0.7, 0.7);
    }

    setRenderingFaceColor(id, c);
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceUV(const Index& id)
{
    typedef typename M::Face Face;
    typedef typename M::Vertex Vertex;
    typedef typename M::UV UV;

    const Face& face = this->vMesh->face(id);

    std::vector<Point2f> uvPoints(face.vertexNumber());
    if (face.hasWedgeUV()) {
        const std::vector<UV>& uvs = face.wedgeUV();
        for (Index j = 0; j < uvs.size(); ++j) {
            uvPoints[j] = Point2f(uvs[j].x(), uvs[j].y());
        }
    }
    else {
        for (Index j = 0; j < face.vertexNumber(); ++j) {
            const Vertex& vertex = this->vMesh->vertex(face.vertexId(j));
            const UV& uv = vertex.uvCoords();
            uvPoints[j] = Point2f(uv.x(), uv.y());
        }
    }

    setRenderingFaceUV(id, uvPoints);
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceWireframeColor(const Index& id)
{
    setRenderingFaceWireframeColor(id, this->wireframeColor());
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceMaterial(const Index& id)
{
    typedef typename M::Face Face;
    const Face& face = this->vMesh->face(id);
    setRenderingFaceMaterial(id, face.materialId());
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaces(const std::vector<std::vector<unsigned int>>& renderingFaces)
{
    this->vRenderingFaces = renderingFaces;
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceNormals(const std::vector<double>& renderingFaceNormals)
{
    this->vRenderingFaceNormals = renderingFaceNormals;
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceUVs(const std::vector<std::vector<float>>& renderingFaceUVs)
{
    this->vRenderingFaceUVs = renderingFaceUVs;
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceWireframeColors(const std::vector<float>& renderingFaceWireframeColors)
{
    this->vRenderingFaceWireframeColors = renderingFaceWireframeColors;
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceMaterials(const std::vector<Index>& renderingFaceMaterials)
{
    this->vRenderingFaceMaterials = renderingFaceMaterials;
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFace(const Index& id, const std::vector<unsigned int>& f)
{
    Index mappedId = vFaceMap[id];
    this->vRenderingFaces[mappedId] = f;
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceNormal(const Index& id, const Vector3d& n)
{
    Index mappedId = vFaceMap[id];
    this->vRenderingFaceNormals[mappedId*3] = n.x();
    this->vRenderingFaceNormals[mappedId*3+1] = n.y();
    this->vRenderingFaceNormals[mappedId*3+2] = n.z();
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceColor(const Index& id, const Color& c)
{
    Index mappedId = vFaceMap[id];
    this->vRenderingFaceColors[mappedId*4] = c.redF();
    this->vRenderingFaceColors[mappedId*4+1] = c.greenF();
    this->vRenderingFaceColors[mappedId*4+2] = c.blueF();
    this->vRenderingFaceColors[mappedId*4+3] = c.alphaF();
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceUV(const Index& id, const std::vector<Point2f>& uvs)
{
    Index mappedId = vFaceMap[id];

    std::vector<float> uvsFloat(uvs.size() * 2);
    for (Index i = 0; i < uvs.size(); ++i) {
        uvsFloat[i*2] = uvs[i].x();
        uvsFloat[i*2+1] = uvs[i].y();
    }

    this->vRenderingFaceUVs[mappedId] = uvsFloat;
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceWireframeColor(const Index& id, const Color& c)
{
    Index mappedId = vFaceMap[id];
    this->vRenderingFaceWireframeColors[mappedId*4] = c.redF();
    this->vRenderingFaceWireframeColors[mappedId*4+1] = c.greenF();
    this->vRenderingFaceWireframeColors[mappedId*4+2] = c.blueF();
    this->vRenderingFaceWireframeColors[mappedId*4+3] = c.alphaF();
}


template<class M>
void FaceMeshDrawer<M>::setRenderingFaceMaterial(const Index& id, const Index& m)
{
    Index mappedId = vFaceMap[id];
    this->vRenderingFaceMaterials[mappedId] = m;
}


template<class M>
void FaceMeshDrawer<M>::loadTextures()
{
    typedef typename M::FaceId FaceId;
    typedef typename M::MaterialId MaterialId;
    typedef typename M::Material Material;
    clearTextures();

    std::unordered_set<MaterialId> usedMaterials;
    for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (!this->vMesh->isFaceDeleted(fId)) {
            MaterialId mId = this->vMesh->face(fId).materialId();
            if (mId != MAX_INDEX) {
                usedMaterials.insert(mId);
            }
        }
    }

    vTextures.resize(this->vMesh->nextMaterialId());
    for (const MaterialId& mId : usedMaterials) {
        const Material& mat = this->vMesh->material(mId);

#ifdef NVL_STB_LOADED
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(mat.diffuseMap().c_str(), &width, &height, &nrChannels, 0);

        if (data != nullptr) {            
            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

            glBindTexture(GL_TEXTURE_2D, 0);

            glDisable(GL_TEXTURE_2D);
            glDisable(GL_BLEND);

            vTextures[mId] = texture;
        }

        stbi_image_free(data);
#else
        vTextures[mId] = nvl::maxLimitValue<unsigned int>();
#endif

    }
}

template<class M>
void FaceMeshDrawer<M>::clearTextures()
{
    for (const unsigned int& texture : vTextures) {
        glDeleteTextures(1, &texture);
    }
    vTextures.clear();
}

template<class M>
void FaceMeshDrawer<M>::drawFaceSmoothShading() const
{
    glEnable(GL_LIGHTING);

    if (this->faceTransparency()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_GREATER, EPSILON);
        glEnable(GL_ALPHA_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    if (this->vTextureVisible) {
        glEnable(GL_TEXTURE_2D);
    }

    glDepthFunc(GL_LESS);
    glDepthRange(0.01, 1.0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glShadeModel(GL_SMOOTH);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, this->vRenderingVertices.data());

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, this->vRenderingVertexNormals.data());

    if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, this->vRenderingVertexColors.data());
    }

    for (Index fId = 0; fId < this->vRenderingFaces.size(); ++fId) {
        if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_FACE) {
            glColor4f(
                this->vRenderingFaceColors[fId*4],
                this->vRenderingFaceColors[fId*4+1],
                this->vRenderingFaceColors[fId*4+2],
                this->vRenderingFaceColors[fId*4+3]);
        }

        if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_UNIFORM) {
            glColor(this->vFaceUniformColor);
        }

        const Index& mId = this->vRenderingFaceMaterials[fId];
        if (this->vTextureVisible && mId != MAX_INDEX && vTextures[mId] != nvl::maxLimitValue<unsigned int>()) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, vTextures[mId]);

            glBegin(GL_POLYGON);
            for (Index j = 0; j < this->vRenderingFaces[fId].size(); ++j) {
                if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
                    glColor4f(
                        this->vRenderingVertexColors[this->vRenderingFaces[fId][j]*4],
                        this->vRenderingVertexColors[this->vRenderingFaces[fId][j]*4+1],
                        this->vRenderingVertexColors[this->vRenderingFaces[fId][j]*4+2],
                        this->vRenderingVertexColors[this->vRenderingFaces[fId][j]*4+3]);
                }

                glNormal3d(
                        this->vRenderingVertexNormals[this->vRenderingFaces[fId][j]*3],
                        this->vRenderingVertexNormals[this->vRenderingFaces[fId][j]*3+1],
                        this->vRenderingVertexNormals[this->vRenderingFaces[fId][j]*3+2]);

                glTexCoord2f(this->vRenderingFaceUVs[fId][j*2], this->vRenderingFaceUVs[fId][j*2+1]);

                glVertex3d(
                    this->vRenderingVertices[this->vRenderingFaces[fId][j]*3],
                    this->vRenderingVertices[this->vRenderingFaces[fId][j]*3+1],
                    this->vRenderingVertices[this->vRenderingFaces[fId][j]*3+2]);
            }
            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else {
            glDrawElements(GL_POLYGON, this->vRenderingFaces[fId].size(), GL_UNSIGNED_INT, this->vRenderingFaces[fId].data());
        }
    }

    if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    if (this->vTextureVisible) {
        glDisable(GL_TEXTURE_2D);
    }

    if (this->faceTransparency()) {
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glDisable(GL_ALPHA_TEST);
    }
}

template<class M>
void FaceMeshDrawer<M>::drawFaceFlatShading() const
{
    glEnable(GL_LIGHTING);

    if (this->vFaceTransparency) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_GREATER, EPSILON);
        glEnable(GL_ALPHA_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    if (this->vTextureVisible) {
        glEnable(GL_TEXTURE_2D);
    }

    glDepthFunc(GL_LESS);
    glDepthRange(0.01, 1.0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glShadeModel(GL_FLAT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, this->vRenderingVertices.data());

    if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_UNIFORM) {
        glColor(this->vFaceUniformColor);
    }

    for (Index fId = 0; fId < this->vRenderingFaces.size(); ++fId) {
        if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_FACE) {
            glColor4f(
                this->vRenderingFaceColors[fId*4],
                this->vRenderingFaceColors[fId*4+1],
                this->vRenderingFaceColors[fId*4+2],
                this->vRenderingFaceColors[fId*4+3]);
        }
        else if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
            float avgRed = 0.0f, avgGreen = 0.0f, avgBlue = 0.0f, avgAlpha = 0.0f;
            for (Index j = 0; j < this->vRenderingFaces[fId].size(); ++j) {
                Index vId = this->vRenderingFaces[fId].at(j);
                avgRed += this->vRenderingVertexColors[vId*4];
                avgGreen += this->vRenderingVertexColors[vId*4+1];
                avgBlue += this->vRenderingVertexColors[vId*4+2];
                avgAlpha += this->vRenderingVertexColors[vId*4+3];
            }
            avgRed /= this->vRenderingFaces[fId].size();
            avgGreen /= this->vRenderingFaces[fId].size();
            avgBlue /= this->vRenderingFaces[fId].size();
            avgAlpha /= this->vRenderingFaces[fId].size();

            glColor4f(
                avgRed,
                avgGreen,
                avgBlue,
                avgAlpha);
        }

        glNormal3d(
            this->vRenderingFaceNormals[fId*3],
            this->vRenderingFaceNormals[fId*3+1],
            this->vRenderingFaceNormals[fId*3+2]);

        const Index& mId = this->vRenderingFaceMaterials[fId];
        if (this->vTextureVisible && mId != MAX_INDEX && vTextures[mId] != nvl::maxLimitValue<unsigned int>()) {
            glBindTexture(GL_TEXTURE_2D, vTextures[mId]);

            glBegin(GL_POLYGON);
            for (Index j = 0; j < this->vRenderingFaces[fId].size(); ++j) {
                glTexCoord2f(this->vRenderingFaceUVs[fId][j*2], this->vRenderingFaceUVs[fId][j*2+1]);

                glVertex3d(
                    this->vRenderingVertices[this->vRenderingFaces[fId][j]*3],
                    this->vRenderingVertices[this->vRenderingFaces[fId][j]*3+1],
                    this->vRenderingVertices[this->vRenderingFaces[fId][j]*3+2]);
            }
            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else {
            glDrawElements(GL_POLYGON, this->vRenderingFaces[fId].size(), GL_UNSIGNED_INT, this->vRenderingFaces[fId].data());
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    if (this->vTextureVisible) {
        glDisable(GL_TEXTURE_2D);
    }

    if (this->vFaceTransparency) {
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glDisable(GL_ALPHA_TEST);
    }
}

template<class M>
void FaceMeshDrawer<M>::drawWireframe() const
{
    glDisable(GL_LIGHTING);

    if (this->vFaceTransparency) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_GREATER, EPSILON);
        glEnable(GL_ALPHA_TEST);
    }

    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDepthRange(0.0, 1.0);
    glDepthFunc(GL_LEQUAL);

    glLineWidth(this->vWireframeSize);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, this->vRenderingVertices.data());

    for (Index fId = 0; fId < this->vRenderingFaces.size(); ++fId) {
        glColor4f(
            this->vRenderingFaceWireframeColors[fId*4],
            this->vRenderingFaceWireframeColors[fId*4+1],
            this->vRenderingFaceWireframeColors[fId*4+2],
            this->vRenderingFaceWireframeColors[fId*4+3]);

        glDrawElements(GL_POLYGON, this->vRenderingFaces[fId].size(), GL_UNSIGNED_INT, this->vRenderingFaces[fId].data());
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (this->vFaceTransparency) {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
    }
}

template<class M>
void FaceMeshDrawer<M>::drawFaceNormals() const
{
    typedef typename M::Point Point;
    typedef typename M::FaceNormal FaceNormal;
    typedef typename M::VertexId VertexId;

    const int subdivision = 5;
    const double arrowRadius = getFaceNormalRadius();
    const double arrowLength = getFaceNormalLength();

    for (Index fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (this->vMesh->isFaceDeleted(fId))
            continue;

        std::vector<Point> points;

        for (Index j = 0; j < this->renderingFace(fId).size(); ++j) {
            VertexId vId = this->renderingFace(fId).at(j);

            Point point(this->vRenderingVertices[vId*3], this->vRenderingVertices[vId*3+1], this->vRenderingVertices[vId*3+2]);

            points.push_back(point);
        }

        FaceNormal normal = this->renderingFaceNormal(fId);

        Point start = barycenter(points);
        Point end = start + (normal * arrowLength);

        if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_FACE) {
            Color color(this->vRenderingFaceColors[fId*4], this->vRenderingFaceColors[fId*4+1], this->vRenderingFaceColors[fId*4+2], this->vRenderingFaceColors[fId*4+3]);
            drawArrow(start, end, arrowRadius, color, subdivision, subdivision);
        }
        else {
            assert(this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_UNIFORM || this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX);
            drawArrow(start, end, arrowRadius, this->vFaceUniformColor, subdivision, subdivision);
        }
    }
}

template<class M>
void FaceMeshDrawer<M>::drawVertexValueShader() const
{
    typedef typename M::VertexId VertexId;
    typedef typename M::Face Face;

    if (vFaceShader == nullptr || this->vVertexValues.empty())
        return;

    vFaceShader->bind();
    int vertexValueAttribute = vFaceShader->attributeLocation("vertex_value");

    vFaceShader->initGL();
    for (const Face& face : this->vMesh->faces()) {
        vFaceShader->initFace(face.id());

        for (const VertexId& vId : face.vertexIds()) {
            vFaceShader->initVertex(vId);

            Point3d p = this->renderingVertex(vId);

            vFaceShader->setAttribute(vertexValueAttribute, this->vVertexValues[vId]);
            vFaceShader->addVertex(vId, p);

            vFaceShader->postVertex(vId);
        }

        vFaceShader->postFace(face.id());
    }
    vFaceShader->postGL();

    vFaceShader->release();
}

template<class M>
void FaceMeshDrawer<M>::drawFaceSmoothShadingWithNames(Canvas* canvas, const Index drawableId) const
{
    typedef typename M::Point Point;
    typedef typename M::VertexId VertexId;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    glEnable(GL_LIGHTING);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDepthFunc(GL_LESS);
    glDepthRange(0.01, 1.0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glShadeModel(GL_SMOOTH);

    for (Index fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (this->vMesh->isFaceDeleted(fId))
            continue;

        float alpha = this->vFaceUniformColor.alphaF();
        if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_FACE) {
            alpha = this->renderingFaceColor(fId).alphaF();
        }
        else if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
            float avgAlpha = 0.0f;
            for (Index j = 0; j < this->renderingFace(fId).size(); ++j) {
                VertexId vId = this->renderingFace(fId).at(j);
                avgAlpha += this->vRenderingVertexColors[vId*4+3];
            }
            alpha = avgAlpha / this->renderingFace(fId).size();
        }
        if (this->faceTransparency() && alpha <= nvl::EPSILON) {
            continue;
        }

        Canvas::PickingData pickingData;
        pickingData.value1 = drawableId;
        pickingData.identifier = Canvas::PICKING_MESH_FACE;
        pickingData.value2 = fId;
        pickingNameMap.push_back(pickingData);

        glPushName(pickingNameMap.size() - 1);
        glBegin(GL_POLYGON);
        glColor(this->faceUniformColor());

        for (Index j = 0; j < this->renderingFace(fId).size(); ++j) {
            VertexId vId = this->renderingFace(fId).at(j);

            Point point(this->vRenderingVertices[vId*3], this->vRenderingVertices[vId*3+1], this->vRenderingVertices[vId*3+2]);
            Point normal(this->vRenderingVertexNormals[vId*3], this->vRenderingVertexNormals[vId*3+1], this->vRenderingVertexNormals[vId*3+2]);

            glNormal(normal);
            glVertex(point);
        }
        glEnd();
        glPopName();
    }

    glDisable(GL_CULL_FACE);
}

template<class M>
void FaceMeshDrawer<M>::drawFaceFlatShadingWithNames(Canvas* canvas, const Index drawableId) const
{
    typedef typename M::Point Point;
    typedef typename M::FaceNormal FaceNormal;
    typedef typename M::VertexId VertexId;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    glEnable(GL_LIGHTING);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDepthFunc(GL_LESS);
    glDepthRange(0.01, 1.0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glShadeModel(GL_FLAT);

    for (Index fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (this->vMesh->isFaceDeleted(fId))
            continue;

        float alpha = this->vFaceUniformColor.alphaF();
        if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_FACE) {
            alpha = this->renderingFaceColor(fId).alphaF();
        }
        else if (this->vFaceColorMode == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
            float avgAlpha = 0.0f;
            for (Index j = 0; j < this->renderingFace(fId).size(); ++j) {
                VertexId vId = this->renderingFace(fId).at(j);
                Color currentColor = this->renderingVertexColor(vId);
                avgAlpha += currentColor.alphaF();
            }
            alpha = avgAlpha / this->renderingFace(fId).size();
        }
        if (this->faceTransparency() && alpha <= nvl::EPSILON) {
            continue;
        }

        FaceNormal normal = this->renderingFaceNormal(fId);

        Canvas::PickingData pickingData;
        pickingData.value1 = drawableId;
        pickingData.identifier = Canvas::PICKING_MESH_FACE;
        pickingData.value2 = fId;
        pickingNameMap.push_back(pickingData);

        glPushName(pickingNameMap.size() - 1);
        glBegin(GL_POLYGON);
        glColor(this->faceUniformColor());
        glNormal(normal);

        for (Index j = 0; j < this->renderingFace(fId).size(); ++j) {
            VertexId vId = this->renderingFace(fId).at(j);

            Point point(this->vRenderingVertices[vId*3], this->vRenderingVertices[vId*3+1], this->vRenderingVertices[vId*3+2]);
            Point normal(this->vRenderingVertexNormals[vId*3], this->vRenderingVertexNormals[vId*3+1], this->vRenderingVertexNormals[vId*3+2]);

            glVertex(point);
        }
        glEnd();
        glPopName();
    }

    glDisable(GL_CULL_FACE);
}

template<class M>
void FaceMeshDrawer<M>::drawWireframeWithNames(Canvas* canvas, const Index drawableId) const
{
    typedef typename M::Point Point;
    typedef typename M::VertexId VertexId;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDepthRange(0.0, 1.0);
    glDepthFunc(GL_LEQUAL);

    glLineWidth(this->vWireframeSize);

    glColor(this->wireframeColor());

    for (Index fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (this->vMesh->isFaceDeleted(fId))
            continue;

        float alpha = this->renderingFaceWireframeColor(fId).alphaF();
        if (this->faceTransparency() && alpha <= nvl::EPSILON) {
            continue;
        }

        for (Index j = 0; j < this->renderingFace(fId).size(); ++j) {
            VertexId vId1 = this->renderingFace(fId).at(j);
            VertexId vId2 = this->renderingFace(fId).at((j+1) % this->renderingFace(fId).size());

            Point point1(this->vRenderingVertices[vId1*3], this->vRenderingVertices[vId1*3+1], this->vRenderingVertices[vId1*3+2]);
            Point point2(this->vRenderingVertices[vId2*3], this->vRenderingVertices[vId2*3+1], this->vRenderingVertices[vId2*3+2]);

            Canvas::PickingData pickingData;
            pickingData.value1 = drawableId;
            pickingData.identifier = Canvas::PICKING_MESH_FACE_EDGE;
            pickingData.value2 = fId;
            pickingData.value3 = j;
            pickingNameMap.push_back(pickingData);

            glPushName(pickingNameMap.size() - 1);
            glBegin(GL_LINES);
            glVertex(point1);
            glVertex(point2);
            glEnd();
            glPopName();
        }
    }

    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

template<class M>
double FaceMeshDrawer<M>::getFaceNormalRadius() const
{
    const double radiusBBoxFactor = 0.0002;
    return this->boundingBox().diagonal().norm() * radiusBBoxFactor * this->vFaceNormalSize;
}

template<class M>
double FaceMeshDrawer<M>::getFaceNormalLength() const
{
    const double lengthBBoxFactor = 0.001;
    return this->boundingBox().diagonal().norm() * lengthBBoxFactor * this->vFaceNormalSize;
}

}
