#include "vertex_mesh_drawer.h"

#include <nvl/viewer/gl/draw_primitives.h>
#include <nvl/viewer/gl/draw_gl.h>

#include <nvl/math/constants.h>

namespace nvl {

template<class M>
VertexMeshDrawer<M>::VertexMeshDrawer() : VertexMeshDrawer<M>(nullptr)
{

}

template<class M>
VertexMeshDrawer<M>::VertexMeshDrawer(M* mesh, const bool visible, const bool pickable) :
    VertexMeshDrawerBase(),
    MeshDrawer<M>(mesh, visible, pickable)
{
    update();
}

template<class M>
void VertexMeshDrawer<M>::draw() const
{
    if (this->vMesh == nullptr)
        return;

    if (this->vVertexVisible) {
        if (this->vVertexShapeMode == VertexMeshDrawerBase::VERTEX_SHAPE_POINT) {
            drawVerticesPoint();
        }
        else {
            assert(this->vVertexShapeMode == VertexMeshDrawerBase::VERTEX_SHAPE_DOT);
            drawVerticesDot();
        }
    }

    if (this->vVertexNormalsVisible) {
        drawVertexNormals();
    }
}

template<class M>
void VertexMeshDrawer<M>::drawWithNames(Canvas* canvas, const Index drawableId) const
{
    if (this->vMesh == nullptr || !this->isPickable())
        return;

    if (this->vVertexVisible) {
        if (this->vVertexShapeMode == VertexMeshDrawerBase::VERTEX_SHAPE_POINT) {
            drawVerticesPointWithNames(canvas, drawableId);
        }
        else {
            assert(this->vVertexShapeMode == VertexMeshDrawerBase::VERTEX_SHAPE_DOT);
            drawVerticesDotWithNames(canvas, drawableId);
        }
    }
}

template<class M>
void VertexMeshDrawer<M>::update()
{
    MeshDrawer<M>::update();

    resetRenderingVertexData();
}

template<class M>
Point3d VertexMeshDrawer<M>::renderingVertex(const Index& id) const
{
    Index mappedId = vVertexMap[id];
    return Point3d(this->vRenderingVertices[mappedId*3], this->vRenderingVertices[mappedId*3+1], this->vRenderingVertices[mappedId*3+2]);
}

template<class M>
Vector3d VertexMeshDrawer<M>::renderingVertexNormal(const Index& id) const
{
    Index mappedId = vVertexMap[id];
    return Vector3d(this->vRenderingVertexNormals[mappedId*3], this->vRenderingVertexNormals[mappedId*3+1], this->vRenderingVertexNormals[mappedId*3+2]);
}

template<class M>
Color VertexMeshDrawer<M>::renderingVertexColor(const Index& id) const
{
    Index mappedId = vVertexMap[id];
    return Color(this->vRenderingVertexColors[mappedId*4], this->vRenderingVertexColors[mappedId*4+1], this->vRenderingVertexColors[mappedId*4+2], this->vRenderingVertexColors[mappedId*4+3]);
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingData()
{
    resetRenderingVertexData();
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertexData()
{
    typedef typename M::Vertex Vertex;

    if (this->vMesh == nullptr)
        return;

    vVertexMap.resize(this->vMesh->nextVertexId(), MAX_INDEX);

    Index i = 0;
    for (const Vertex& vertex : this->vMesh->vertices()) {
        vVertexMap[vertex.id()] = i;
        ++i;
    }

    resetRenderingVertices();
    resetRenderingVertexNormals();
    resetRenderingVertexColors();
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertices()
{
    typedef typename M::VertexId VertexId;

    this->vRenderingVertices.resize(this->vMesh->vertexNumber() * 3);

    #pragma omp parallel for
    for (VertexId vId = 0; vId < this->vMesh->nextVertexId(); ++vId) {
        if (!this->vMesh->isVertexDeleted(vId)) {
            resetRenderingVertex(vId);
        }
    }
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertexNormals()
{
    typedef typename M::VertexId VertexId;

    this->vRenderingVertexNormals.resize(this->vMesh->vertexNumber() * 3);

    #pragma omp parallel for
    for (VertexId vId = 0; vId < this->vMesh->nextVertexId(); ++vId) {
        if (!this->vMesh->isVertexDeleted(vId)) {
            resetRenderingVertexNormal(vId);
        }
    }
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertexColors()
{
    typedef typename M::VertexId VertexId;

    this->vRenderingVertexColors.resize(this->vMesh->vertexNumber() * 4);

    #pragma omp parallel for
    for (VertexId vId = 0; vId < this->vMesh->nextVertexId(); ++vId) {
        if (!this->vMesh->isVertexDeleted(vId)) {
            resetRenderingVertexColor(vId);
        }
    }
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertex(const Index& id)
{
    typedef typename M::Vertex Vertex;
    const Vertex& vertex = this->vMesh->vertex(id);
    setRenderingVertex(id, vertex.point());
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertexNormal(const Index& id)
{
    typedef typename M::Vertex Vertex;
    const Vertex& vertex = this->vMesh->vertex(id);
    setRenderingVertexNormal(id, vertex.normal());
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertexColor(const Index& id)
{
    typedef typename M::Vertex Vertex;
    const Vertex& vertex = this->vMesh->vertex(id);
    setRenderingVertexColor(id, vertex.color());
}

template<class M>
void VertexMeshDrawer<M>::setRenderingVertices(const std::vector<double>& renderingVertices)
{
    this->vRenderingVertices = renderingVertices;
}

template<class M>
void VertexMeshDrawer<M>::setRenderingVertexNormals(const std::vector<double>& renderingVertexNormals)
{
    this->vRenderingVertexNormals = renderingVertexNormals;
}

template<class M>
void VertexMeshDrawer<M>::setRenderingVertexColors(const std::vector<float>& renderingVertexColors)
{
    this->vRenderingVertexColors = renderingVertexColors;
}

template<class M>
void VertexMeshDrawer<M>::setRenderingVertex(const Index& id, const Point3d& p)
{
    Index mappedId = vVertexMap[id];
    this->vRenderingVertices[mappedId*3] = p.x();
    this->vRenderingVertices[mappedId*3+1] = p.y();
    this->vRenderingVertices[mappedId*3+2] = p.z();
}

template<class M>
void VertexMeshDrawer<M>::setRenderingVertexNormal(const Index& id, const Vector3d& n)
{
    Index mappedId = vVertexMap[id];
    this->vRenderingVertexNormals[mappedId*3] = n.x();
    this->vRenderingVertexNormals[mappedId*3+1] = n.y();
    this->vRenderingVertexNormals[mappedId*3+2] = n.z();
}

template<class M>
void VertexMeshDrawer<M>::setRenderingVertexColor(const Index& id, const Color &c)
{
    Index mappedId = vVertexMap[id];
    this->vRenderingVertexColors[mappedId*4] = c.redF();
    this->vRenderingVertexColors[mappedId*4+1] = c.greenF();
    this->vRenderingVertexColors[mappedId*4+2] = c.blueF();
    this->vRenderingVertexColors[mappedId*4+3] = c.alphaF();
}

template<class M>
void VertexMeshDrawer<M>::drawVerticesPoint() const
{
    glDisable(GL_LIGHTING);

    if (this->vertexTransparency()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_GREATER, EPSILON);
        glEnable(GL_ALPHA_TEST);
    }

    glPointSize(this->vVertexSize);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, this->vRenderingVertices.data());

    if (this->vVertexColorMode == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, this->vRenderingVertexColors.data());
    }
    else {
        assert(this->vVertexColorMode == VertexMeshDrawerBase::VERTEX_COLOR_UNIFORM);
        glColor(this->vVertexUniformColor);
    }

    glDrawArrays(GL_POINTS, 0, this->vRenderingVertices.size() / 3);

    if (this->vVertexColorMode == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
        glDisableClientState(GL_COLOR_ARRAY);
    }
    glDisableClientState(GL_VERTEX_ARRAY);

    if (this->vertexTransparency()) {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
    }
}

template<class M>
void VertexMeshDrawer<M>::drawVerticesDot() const
{
    typedef typename M::Point Point;

    if (this->vertexTransparency()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_GREATER, EPSILON);
        glEnable(GL_ALPHA_TEST);
    }

    const int subdivision = 20;
    const double bboxFactor = 0.001;
    const double vertexSize = this->vVertexSize * (this->boundingBox().diagonal().norm() * bboxFactor);

    for (Index i = 0; i < this->vRenderingVertices.size()/3; i++) {
        Point point(this->vRenderingVertices[i*3], this->vRenderingVertices[i*3+1], this->vRenderingVertices[i*3+2]);

        if (this->vVertexColorMode == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
            Color color(this->vRenderingVertexColors[i*4], this->vRenderingVertexColors[i*4+1], this->vRenderingVertexColors[i*4+2], this->vRenderingVertexColors[i*4+3]);
            drawSphere(point, vertexSize, color, subdivision, subdivision);
        }
        else {
            assert(this->vVertexColorMode == VertexMeshDrawerBase::VERTEX_COLOR_UNIFORM);
            drawSphere(point, vertexSize, this->vVertexUniformColor, subdivision, subdivision);
        }
    }

    if (this->vertexTransparency()) {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
    }
}

template<class M>
void VertexMeshDrawer<M>::drawVertexNormals() const
{
    typedef typename M::Point Point;
    typedef typename M::VertexNormal VertexNormal;

    if (this->vertexTransparency()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_GREATER, EPSILON);
        glEnable(GL_ALPHA_TEST);
    }

    const int subdivision = 5;
    const double arrowRadius = getVertexNormalRadius();
    const double arrowLength = getVertexNormalLength();

    for (Index i = 0; i < this->vRenderingVertices.size()/3; i++) {
        Point point(this->vRenderingVertices[i*3], this->vRenderingVertices[i*3+1], this->vRenderingVertices[i*3+2]);
        VertexNormal normal(this->vRenderingVertexNormals[i*3], this->vRenderingVertexNormals[i*3+1], this->vRenderingVertexNormals[i*3+2]);

        Point end = point + (normal  * arrowLength);

        if (this->vVertexColorMode == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
            Color color(this->vRenderingVertexColors[i*4], this->vRenderingVertexColors[i*4+1], this->vRenderingVertexColors[i*4+2], this->vRenderingVertexColors[i*4+3]);
            drawArrow(point, end, arrowRadius, color, subdivision, subdivision);
        }
        else {
            assert(this->vVertexColorMode == VertexMeshDrawerBase::VERTEX_COLOR_UNIFORM);
            drawArrow(point, end, arrowRadius, this->vVertexUniformColor, subdivision, subdivision);
        }
    }

    if (this->vertexTransparency()) {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
    }
}

template<class M>
void VertexMeshDrawer<M>::drawVerticesPointWithNames(Canvas* canvas, const Index drawableId) const
{
    typedef typename M::Point Point;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    glPointSize(this->vVertexSize);

    glBegin(GL_POINTS);

    for (Index vId = 0; vId < this->vMesh->nextVertexId(); ++vId) {
        if (this->vMesh->isVertexDeleted(vId))
            continue;

        Point point = this->renderingVertex(vId);

        float alpha = this->vVertexUniformColor.alphaF();
        if (this->vVertexColorMode == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
            alpha = this->renderingVertexColor(vId).alphaF();
        }
        if (this->vertexTransparency() && alpha <= nvl::EPSILON) {
            continue;
        }

        Canvas::PickingData pickingData;
        pickingData.value1 = drawableId;
        pickingData.identifier = Canvas::PICKING_MESH_VERTEX;
        pickingData.value2 = vId;
        pickingNameMap.push_back(pickingData);

        glPushName(pickingNameMap.size() - 1);
        glVertex(point);
        glPopName();
    }

    glEnd();
}

template<class M>
void VertexMeshDrawer<M>::drawVerticesDotWithNames(Canvas* canvas, const Index drawableId) const
{
    typedef typename M::Point Point;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    const int subdivision = 20;
    const double vertexSize = getVertexSize();

    for (Index vId = 0; vId < this->vMesh->nextVertexId(); ++vId) {
        if (this->vMesh->isVertexDeleted(vId))
            continue;

        Point point = this->renderingVertex(vId);

        float alpha = this->vVertexUniformColor.alphaF();
        if (this->vVertexColorMode == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
            alpha = this->renderingVertexColor(vId).alphaF();
        }
        if (this->vertexTransparency() && alpha <= nvl::EPSILON) {
            continue;
        }

        Canvas::PickingData pickingData;
        pickingData.value1 = drawableId;
        pickingData.identifier = Canvas::PICKING_MESH_VERTEX;
        pickingData.value2 = vId;
        pickingNameMap.push_back(pickingData);

        glPushName(pickingNameMap.size() - 1);
        drawSphere(point, vertexSize, this->vVertexUniformColor, subdivision, subdivision);
        glPopName();
    }

    glEnd();
}

template<class M>
double VertexMeshDrawer<M>::getVertexSize() const
{
    const double bboxFactor = 0.001;
    return this->vVertexSize * (this->boundingBox().diagonal().norm() * bboxFactor);
}

template<class M>
double VertexMeshDrawer<M>::getVertexNormalRadius() const
{
    const double radiusBBoxFactor = 0.001;
    return this->boundingBox().diagonal().norm() * radiusBBoxFactor * this->vVertexNormalSize;
}

template<class M>
double VertexMeshDrawer<M>::getVertexNormalLength() const
{
    const double lengthBBoxFactor = 0.001;
    return this->boundingBox().diagonal().norm() * lengthBBoxFactor * this->vVertexNormalSize;
}

}
