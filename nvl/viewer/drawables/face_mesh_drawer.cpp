#include "face_mesh_drawer.h"

#ifdef NVL_OPENGL_LOADED

#include <nvl/viewer/gl/gl_primitives.h>
#include <nvl/viewer/gl/gl_draw.h>
#include <nvl/viewer/gl/gl_textures.h>

#include <nvl/math/constants.h>
#include <nvl/math/numeric_limits.h>

#include <nvl/models/algorithms/mesh_geometric_information.h>


namespace nvl {

template<class M>
FaceMeshDrawer<M>::FaceMeshDrawer() : FaceMeshDrawer<M>(nullptr)
{

}

template<class M>
FaceMeshDrawer<M>::FaceMeshDrawer(M* mesh, const bool visible, const bool pickable) :
    FaceMeshDrawerBase(),
    PolylineMeshDrawer<M>(mesh, visible, pickable),
    vDefaultFaceColor(0.7, 0.7, 0.7)
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

    if (this->faceVisible()) {
        if (this->faceShadingMode() == FaceMeshDrawerBase::FACE_SHADING_SMOOTH) {
            drawFaceSmoothShading();
        }
        else if (this->faceShadingMode() == FaceMeshDrawerBase::FACE_SHADING_FLAT) {
            drawFaceFlatShading();
        }

        if (this->faceShader() != nullptr) {
            drawShader();
        }
    }

    if (this->wireframeVisible()) {
        drawWireframe();
    }

    if (this->faceNormalVisible()) {
        drawFaceNormals();
    }

}

template<class M>
void FaceMeshDrawer<M>::drawWithNames(Canvas* canvas, const Index drawableId) const
{
    if (this->vMesh == nullptr || !this->isPickable())
        return;

    PolylineMeshDrawer<M>::drawWithNames(canvas, drawableId);

    if (this->faceVisible()) {
        drawFaceWithNames(canvas, drawableId);
    }

    if (this->wireframeVisible()) {
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
bool FaceMeshDrawer<M>::hasFaceNormals() const {
    return !this->vRenderingFaceNormals.empty();
}

template<class M>
bool FaceMeshDrawer<M>::hasFaceColors() const {
    return !this->vRenderingFaceColors.empty();
}

template<class M>
bool FaceMeshDrawer<M>::hasTextures() const {
    return !this->vTextures.empty();
}

template<class M>
bool FaceMeshDrawer<M>::hasFaceTransparency() const
{
    return true;
}

template<class M>
bool FaceMeshDrawer<M>::hasFaceLighting() const
{
    return true;
}

template<class M>
bool FaceMeshDrawer<M>::hasSmoothShading() const
{
    return true;
}

template<class M>
bool FaceMeshDrawer<M>::hasFlatShading() const
{
    return true;
}

template<class M>
bool FaceMeshDrawer<M>::hasVertexColors() const
{
    return VertexMeshDrawer<M>::hasVertexColors();
}

template<class M>
const std::vector<unsigned int>& FaceMeshDrawer<M>::renderingFace(const Index &id) const
{
    const Index& mappedId = vFaceMap[id];
    return this->vRenderingFaces[mappedId];
}

template<class M>
Vector3d FaceMeshDrawer<M>::renderingFaceNormal(const Index& id) const
{
    const Index& mappedId = vFaceMap[id];
    return Vector3d(this->vRenderingFaceNormals[mappedId*3], this->vRenderingFaceNormals[mappedId*3+1], this->vRenderingFaceNormals[mappedId*3+2]);
}

template<class M>
Color FaceMeshDrawer<M>::renderingFaceColor(const Index& id) const
{
    const Index& mappedId = vFaceMap[id];
    return Color(this->vRenderingFaceColors[mappedId*4], this->vRenderingFaceColors[mappedId*4+1], this->vRenderingFaceColors[mappedId*4+2], this->vRenderingFaceColors[mappedId*4+3]);
}
template<class M>
Color FaceMeshDrawer<M>::renderingFaceWireframeColor(const Index& id) const
{
    const Index& mappedId = vFaceMap[id];
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

    vFaceMap.resize(this->vMesh->faceNumber(), NULL_ID);

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
        if (this->vMesh->isFaceDeleted(fId))
            continue;

        resetRenderingFace(fId);
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceNormals()
{
    typedef typename M::FaceId FaceId;

    if (this->vMesh->hasFaceNormals()) {
        this->vRenderingFaceNormals.resize(this->vMesh->faceNumber() * 3);

        #pragma omp parallel for
        for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
            if (this->vMesh->isFaceDeleted(fId))
                continue;

            resetRenderingFaceNormal(fId);
        }
    }
    else {
        this->vRenderingFaceNormals.clear();
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceColors()
{
    typedef typename M::FaceId FaceId;

    if (this->vMesh->hasFaceMaterials()) {
        this->vRenderingFaceColors.resize(this->vMesh->faceNumber() * 4);

        #pragma omp parallel for
        for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
            if (this->vMesh->isFaceDeleted(fId))
                continue;

            resetRenderingFaceColor(fId);
        }
    }
    else {
        this->vRenderingFaceColors.clear();
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceUVs()
{
    typedef typename M::FaceId FaceId;    

    if (this->vMesh->hasVertexUVs() || this->vMesh->hasWedgeUVs()) {
        this->vRenderingFaceUVs.resize(this->vMesh->faceNumber());

        #pragma omp parallel for
        for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
            if (this->vMesh->isFaceDeleted(fId))
                continue;

            resetRenderingFaceUV(fId);
        }
    }
    else {
        this->vRenderingFaceUVs.clear();
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceWireframeColors()
{
    typedef typename M::FaceId FaceId;

    this->vRenderingFaceWireframeColors.resize(this->vMesh->faceNumber() * 4);

    #pragma omp parallel for
    for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (this->vMesh->isFaceDeleted(fId))
            continue;

        resetRenderingFaceWireframeColor(fId);
    }
}


template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceMaterials()
{
    typedef typename M::FaceId FaceId;

    if (this->vMesh->hasFaceMaterials()) {
        this->vRenderingFaceMaterials.resize(this->vMesh->faceNumber());

        #pragma omp parallel for
        for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
            if (this->vMesh->isFaceDeleted(fId))
                continue;

            resetRenderingFaceMaterial(fId);
        }
    }
    else {
        this->vRenderingFaceMaterials.clear();
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
    typedef typename M::FaceNormal FaceNormal;
    const FaceNormal& normal = this->vMesh->faceNormal(id);
    setRenderingFaceNormal(id, normal);
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceColor(const Index& id)
{
    typedef typename M::Material Material;
    typedef typename M::MaterialColor MaterialColor;

    if (!this->vMesh->faceMaterialIsNull(id)) {
        assert(this->vMesh->faceMaterial(id) < this->vMesh->nextMaterialId());
        const Material& material = this->vMesh->material(this->vMesh->faceMaterial(id));

        MaterialColor c = material.diffuseColor();
        c.setAlphaF(1.0 - material.transparency());

        setRenderingFaceColor(id, c);
    }
    else {                
        setRenderingFaceColor(id, vDefaultFaceColor);
    }
}

template<class M>
void FaceMeshDrawer<M>::resetRenderingFaceUV(const Index& id)
{
    typedef typename M::Face Face;
    typedef typename M::VertexUV VertexUV;

    const Face& face = this->vMesh->face(id);

    std::vector<Point2f> uvPoints(face.vertexNumber(), Point2f(0.0, 0.0));
    if (this->vMesh->hasWedgeUVs() && !this->vMesh->faceWedgeUVsAreNull(id)) {
        const std::vector<Index>& wedgeUVIds = this->vMesh->faceWedgeUVs(id);

        for (Index j = 0; j < face.vertexNumber(); ++j) {
            if (wedgeUVIds[j] != nvl::NULL_ID) {
                const VertexUV& uv = this->vMesh->wedgeUV(wedgeUVIds[j]);
                uvPoints[j] = Point2f(uv.x(), uv.y());
            }
            else if (this->vMesh->hasVertexUVs()) {
                const VertexUV& uv = this->vMesh->vertexUV(face.vertexId(j));
                uvPoints[j] = Point2f(uv.x(), uv.y());
            }
        }
    }
    else if (this->vMesh->hasVertexUVs()) {
        for (Index j = 0; j < face.vertexNumber(); ++j) {
            const VertexUV& uv = this->vMesh->vertexUV(face.vertexId(j));
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
    setRenderingFaceMaterial(id, this->vMesh->faceMaterial(id));
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
    const Index& mappedId = vFaceMap[id];
    this->vRenderingFaces[mappedId] = f;
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceNormal(const Index& id, const Vector3d& n)
{
    const Index& mappedId = vFaceMap[id];
    this->vRenderingFaceNormals[mappedId*3] = n.x();
    this->vRenderingFaceNormals[mappedId*3+1] = n.y();
    this->vRenderingFaceNormals[mappedId*3+2] = n.z();
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceColor(const Index& id, const Color& c)
{
    const Index& mappedId = vFaceMap[id];
    this->vRenderingFaceColors[mappedId*4] = c.redF();
    this->vRenderingFaceColors[mappedId*4+1] = c.greenF();
    this->vRenderingFaceColors[mappedId*4+2] = c.blueF();
    this->vRenderingFaceColors[mappedId*4+3] = c.alphaF();
}

template<class M>
void FaceMeshDrawer<M>::setRenderingFaceUV(const Index& id, const std::vector<Point2f>& uvs)
{
    const Index& mappedId = vFaceMap[id];

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
    const Index& mappedId = vFaceMap[id];
    this->vRenderingFaceWireframeColors[mappedId*4] = c.redF();
    this->vRenderingFaceWireframeColors[mappedId*4+1] = c.greenF();
    this->vRenderingFaceWireframeColors[mappedId*4+2] = c.blueF();
    this->vRenderingFaceWireframeColors[mappedId*4+3] = c.alphaF();
}


template<class M>
void FaceMeshDrawer<M>::setRenderingFaceMaterial(const Index& id, const Index& m)
{
    const Index& mappedId = vFaceMap[id];
    this->vRenderingFaceMaterials[mappedId] = m;
}

template<class M>
void FaceMeshDrawer<M>::loadTextures()
{
    typedef typename M::FaceId FaceId;
    typedef typename M::MaterialId MaterialId;
    typedef typename M::Material Material;

    clearTextures();

    if (this->vMesh->hasFaceMaterials()) {
        std::unordered_set<MaterialId> usedMaterials;
        for (FaceId fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
            if (this->vMesh->isFaceDeleted(fId))
                continue;

            if (!this->vMesh->faceMaterialIsNull(fId)) {
                const MaterialId& mId = this->vMesh->faceMaterial(fId);
                usedMaterials.insert(mId);
            }
        }

        vTextures.resize(this->vMesh->nextMaterialId());
        for (const MaterialId& mId : usedMaterials) {
            const Material& mat = this->vMesh->material(mId);

            int texture = nvl::maxLimitValue<unsigned int>();
            if (!mat.diffuseMap().empty()) {
                GLint glTextureMode;
                if (this->textureMode() == TextureMode::TEXTURE_MODE_MODULATE) {
                    glTextureMode = GL_MODULATE;
                }
                else {
                    assert(this->textureMode() == TextureMode::TEXTURE_MODE_REPLACE);
                    glTextureMode = GL_REPLACE;
                }
                texture = glLoadTextureImage(mat.diffuseMap(), glTextureMode);
            }
            vTextures[mId] = texture;
        }
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
    const bool textureVisible = this->textureVisible() && !vRenderingFaceMaterials.empty();

    if (this->faceLighting()) {
        glEnable(GL_LIGHTING);
    }
    else {
        glDisable(GL_LIGHTING);
    }

    if (this->faceTransparency()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_GREATER, EPSILON);
        glEnable(GL_ALPHA_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    if (textureVisible) {
        glEnable(GL_TEXTURE_2D);
    }

    glDepthFunc(GL_LESS);
    glDepthRange(0.0001, 1.0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glShadeModel(GL_SMOOTH);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, this->vRenderingVertices.data());

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, this->vRenderingVertexNormals.data());

    if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_UNIFORM) {
        glColor(this->faceUniformColor());
    }
    else if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
        if (!this->vRenderingVertexColors.empty()) {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, 0, this->vRenderingVertexColors.data());
        }
        else {
            glColor(this->vDefaultVertexColor);
        }
    }

    for (Index fId = 0; fId < this->vRenderingFaces.size(); ++fId) {
        if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_FACE) {
            if (!this->vRenderingFaceColors.empty()) {
                glColor4f(
                    this->vRenderingFaceColors[fId*4],
                    this->vRenderingFaceColors[fId*4+1],
                    this->vRenderingFaceColors[fId*4+2],
                    this->vRenderingFaceColors[fId*4+3]);
            }
            else {
                glColor(vDefaultFaceColor);
            }
        }

        if (textureVisible) {
            assert(!this->vRenderingFaceMaterials.empty());
            const Index& mId = this->vRenderingFaceMaterials[fId];

            if (mId != NULL_ID && vTextures[mId] != maxLimitValue<unsigned int>()) {
                glBindTexture(GL_TEXTURE_2D, vTextures[mId]);

                glBegin(GL_POLYGON);
                for (Index j = 0; j < this->vRenderingFaces[fId].size(); ++j) {
                    if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
                        if (!this->vRenderingVertexColors.empty()) {                           
			                glColor4f(
			                    this->vRenderingVertexColors[this->vRenderingFaces[fId][j]*4],
			                    this->vRenderingVertexColors[this->vRenderingFaces[fId][j]*4+1],
			                    this->vRenderingVertexColors[this->vRenderingFaces[fId][j]*4+2],
			                    this->vRenderingVertexColors[this->vRenderingFaces[fId][j]*4+3]);
                        }
                        else {
                            glColor(this->vDefaultVertexColor);
                        }
                    }

                    if (!this->vRenderingFaceNormals.empty()) {
                        glNormal3d(
                                this->vRenderingVertexNormals[this->vRenderingFaces[fId][j]*3],
                                this->vRenderingVertexNormals[this->vRenderingFaces[fId][j]*3+1],
                                this->vRenderingVertexNormals[this->vRenderingFaces[fId][j]*3+2]);
                    }

                    if (!this->vRenderingFaceUVs.empty()) {
                        glTexCoord2f(
                        		this->vRenderingFaceUVs[fId][j*2], this->vRenderingFaceUVs[fId][j*2+1]);
                    }

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
        else {
            glDrawElements(GL_POLYGON, this->vRenderingFaces[fId].size(), GL_UNSIGNED_INT, this->vRenderingFaces[fId].data());
        }
    }

    if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    if (textureVisible) {
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
    const bool textureVisible = this->textureVisible() && !vRenderingFaceMaterials.empty();

    if (this->faceLighting()) {
        glEnable(GL_LIGHTING);
    }
    else {
        glDisable(GL_LIGHTING);
    }

    if (this->faceTransparency()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_GREATER, EPSILON);
        glEnable(GL_ALPHA_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    if (textureVisible) {
        glEnable(GL_TEXTURE_2D);
    }

    glDepthFunc(GL_LESS);
    glDepthRange(0.0001, 1.0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glShadeModel(GL_FLAT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, this->vRenderingVertices.data());

    if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_UNIFORM) {
        glColor(this->faceUniformColor());
    }

    for (Index fId = 0; fId < this->vRenderingFaces.size(); ++fId) {
        if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_FACE) {
            if (!this->vRenderingFaceColors.empty()) {
                glColor4f(
                    this->vRenderingFaceColors[fId*4],
                    this->vRenderingFaceColors[fId*4+1],
                    this->vRenderingFaceColors[fId*4+2],
                    this->vRenderingFaceColors[fId*4+3]);
            }
            else {
                glColor(vDefaultFaceColor);
            }
        }
        else if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
            float avgRed = 0.0f, avgGreen = 0.0f, avgBlue = 0.0f, avgAlpha = 0.0f;

            if (!this->vRenderingVertexColors.empty()) {
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
            }
            else {
                avgRed = this->vDefaultVertexColor.redF();
                avgGreen = this->vDefaultVertexColor.greenF();
                avgBlue = this->vDefaultVertexColor.blueF();
                avgAlpha = this->vDefaultVertexColor.alphaF();
            }

            glColor4f(
                avgRed,
                avgGreen,
                avgBlue,
                avgAlpha);
        }

        if (!this->vRenderingFaceNormals.empty()) {
            glNormal3d(
                this->vRenderingFaceNormals[fId*3],
                this->vRenderingFaceNormals[fId*3+1],
                this->vRenderingFaceNormals[fId*3+2]);
        }

        if (textureVisible) {
            assert(!this->vRenderingFaceMaterials.empty());
            const Index& mId = this->vRenderingFaceMaterials[fId];

            if (mId != NULL_ID && vTextures[mId] != maxLimitValue<unsigned int>()) {
                glBindTexture(GL_TEXTURE_2D, vTextures[mId]);

                glBegin(GL_POLYGON);
                for (Index j = 0; j < this->vRenderingFaces[fId].size(); ++j) {

                    if (!this->vRenderingFaceUVs.empty()) {
                        glTexCoord2f(
                        		this->vRenderingFaceUVs[fId][j*2], this->vRenderingFaceUVs[fId][j*2+1]);
                    }

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
        else {
            glDrawElements(GL_POLYGON, this->vRenderingFaces[fId].size(), GL_UNSIGNED_INT, this->vRenderingFaces[fId].data());
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    if (textureVisible) {
        glDisable(GL_TEXTURE_2D);
    }

    if (this->faceTransparency()) {
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glDisable(GL_ALPHA_TEST);
    }
}

template<class M>
void FaceMeshDrawer<M>::drawWireframe() const
{
    glDisable(GL_LIGHTING);

    if (this->faceTransparency()) {
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

    glLineWidth(this->wireframeSize());

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

    if (this->faceTransparency()) {
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

    if (this->vRenderingFaceNormals.empty())
        return;

    const int subdivision = 5;
    const double arrowRadius = getFaceNormalRadius();
    const double arrowLength = getFaceNormalLength();

    if (this->vRenderingFaceNormals.empty())
        return;

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

        if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_FACE) {
            Color color;

            if (!this->vRenderingFaceColors.empty()) {
                color = Color(this->vRenderingFaceColors[fId*4], this->vRenderingFaceColors[fId*4+1], this->vRenderingFaceColors[fId*4+2], this->vRenderingFaceColors[fId*4+3]);
            }
            else {
                color = vDefaultFaceColor;
            }

            drawArrow(start, end, arrowRadius, color, subdivision, subdivision);
        }
        else {
            assert(this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_UNIFORM || this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX);
            drawArrow(start, end, arrowRadius, this->faceUniformColor(), subdivision, subdivision);
        }
    }
}

template<class M>
void FaceMeshDrawer<M>::drawFaceWithNames(Canvas* canvas, const Index drawableId) const
{
    typedef typename M::Point Point;
    typedef typename M::VertexId VertexId;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    if (this->faceLighting()) {
        glEnable(GL_LIGHTING);
    }
    else {
        glDisable(GL_LIGHTING);
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDepthFunc(GL_LESS);
    glDepthRange(0.0001, 1.0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glShadeModel(GL_FLAT);

    for (Index fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (this->vMesh->isFaceDeleted(fId))
            continue;

        float alpha = this->faceUniformColor().alphaF();
        if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_FACE) {
            if (!this->vRenderingFaceColors.empty()) {
                alpha = this->renderingFaceColor(fId).alphaF();
            }
            else {
                alpha = vDefaultFaceColor.alphaF();
            }
        }
        else if (this->faceColorMode() == FaceMeshDrawerBase::FACE_COLOR_PER_VERTEX) {
            float avgAlpha = 0.0f;

            if (!this->vRenderingVertexColors.empty()) {
                for (Index j = 0; j < this->renderingFace(fId).size(); ++j) {
                    VertexId vId1 = this->renderingFace(fId).at(j);
                    Color currentColor = this->renderingVertexColor(vId1);
                    avgAlpha += currentColor.alphaF();
                }
                avgAlpha /= this->renderingFace(fId).size();
            }
            else {
                avgAlpha = this->vDefaultVertexColor.alphaF();
            }

            alpha = avgAlpha;
        }

        if (this->faceTransparency() && alpha <= EPSILON) {
            continue;
        }

        Canvas::PickingData pickingData;
        pickingData.identifier = Canvas::PICKING_MESH_FACE;

        pickingData.addValue(drawableId);
        pickingData.addValue(fId);

        pickingNameMap.push_back(pickingData);

        glPushName(pickingNameMap.size() - 1);
        glBegin(GL_POLYGON);
        glColor(this->faceUniformColor());

        for (Index j = 0; j < this->renderingFace(fId).size(); ++j) {
            VertexId vId = this->renderingFace(fId).at(j);

            Point point(this->vRenderingVertices[vId*3], this->vRenderingVertices[vId*3+1], this->vRenderingVertices[vId*3+2]);

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

    glLineWidth(this->wireframeSize());

    glColor(this->wireframeColor());

    for (Index fId = 0; fId < this->vMesh->nextFaceId(); ++fId) {
        if (this->vMesh->isFaceDeleted(fId))
            continue;

        float alpha = this->renderingFaceWireframeColor(fId).alphaF();
        if (this->faceTransparency() && alpha <= EPSILON) {
            continue;
        }

        for (Index j = 0; j < this->renderingFace(fId).size(); ++j) {
            VertexId vId1 = this->renderingFace(fId).at(j);
            VertexId vId2 = this->renderingFace(fId).at((j+1) % this->renderingFace(fId).size());

            Point point1(this->vRenderingVertices[vId1*3], this->vRenderingVertices[vId1*3+1], this->vRenderingVertices[vId1*3+2]);
            Point point2(this->vRenderingVertices[vId2*3], this->vRenderingVertices[vId2*3+1], this->vRenderingVertices[vId2*3+2]);

            Canvas::PickingData pickingData;
            pickingData.identifier = Canvas::PICKING_MESH_FACE_EDGE;

            pickingData.addValue(drawableId);
            pickingData.addValue(fId);
            pickingData.addValue(j);

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
void FaceMeshDrawer<M>::drawShader() const
{
    typedef typename M::VertexId VertexId;
    typedef typename M::Face Face;

    GLShader* faceShader = this->faceShader();

    if (faceShader == nullptr)
        return;

    faceShader->bind();

    faceShader->initGL();
    for (const Face& face : this->vMesh->faces()) {
        faceShader->initFace(face.id());

        for (const VertexId& vId : face.vertexIds()) {
            faceShader->initVertex(vId);

            Point3d p = this->renderingVertex(vId);
            faceShader->addVertex(vId, p);

            faceShader->postVertex(vId);
        }

        faceShader->postFace(face.id());
    }
    faceShader->postGL();

    faceShader->release();
}

template<class M>
double FaceMeshDrawer<M>::getFaceNormalRadius() const
{
    const double radiusBBoxFactor = 0.0002;
    return this->boundingBox().diagonal().norm() * radiusBBoxFactor * this->faceNormalSize();
}

template<class M>
double FaceMeshDrawer<M>::getFaceNormalLength() const
{
    const double lengthBBoxFactor = 0.005;
    return this->boundingBox().diagonal().norm() * lengthBBoxFactor * this->faceNormalSize();
}

}

#endif
