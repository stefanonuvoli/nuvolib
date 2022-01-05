#include "vertex_mesh_drawer.h"

#ifdef NVL_OPENGL_LOADED

#include <nvl/math/constants.h>

namespace nvl {

template<class M>
VertexMeshDrawer<M>::VertexMeshDrawer() : VertexMeshDrawer<M>(nullptr)
{

}

template<class M>
VertexMeshDrawer<M>::VertexMeshDrawer(M* mesh, const bool visible, const bool pickable) :
    VertexMeshDrawerBase(),
    MeshDrawer<M>(mesh, visible, pickable),
    vDefaultVertexColor(1.0, 1.0, 1.0)
{
    resetRenderingVertexData();
}

template<class M>
void VertexMeshDrawer<M>::draw() const
{
    if (this->vMesh == nullptr)
        return;

    if (this->vertexVisible()) {
        if (this->vertexShapeMode() == VertexMeshDrawerBase::VERTEX_SHAPE_POINT) {
            drawVerticesPoint();
        }
        else {
            assert(this->vertexShapeMode() == VertexMeshDrawerBase::VERTEX_SHAPE_DOT);
            drawVerticesDot();
        }
    }

    if (this->vertexNormalsVisible()) {
        drawVertexNormals();
    }
}

template<class M>
void VertexMeshDrawer<M>::drawWithNames(Canvas* canvas, const Index drawableId) const
{
    if (this->vMesh == nullptr || !this->isPickable())
        return;

    if (this->vertexVisible()) {
        if (this->vertexShapeMode() == VertexMeshDrawerBase::VERTEX_SHAPE_POINT) {
            drawVerticesPointWithNames(canvas, drawableId);
        }
        else {
            assert(this->vertexShapeMode() == VertexMeshDrawerBase::VERTEX_SHAPE_DOT);
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
bool VertexMeshDrawer<M>::hasVertexNormals() const {
    return !this->vRenderingVertexNormals.empty();
}

template<class M>
bool VertexMeshDrawer<M>::hasVertexColors() const {
    return !this->vRenderingVertexColors.empty();
}

template<class M>
bool VertexMeshDrawer<M>::hasVertexTransparency() const
{
    return true;
}

template<class M>
Point3d VertexMeshDrawer<M>::renderingVertex(const Index& id) const
{
    const Index& mappedId = vVertexMap[id];
    return Point3d(this->vRenderingVertices[mappedId*3], this->vRenderingVertices[mappedId*3+1], this->vRenderingVertices[mappedId*3+2]);
}

template<class M>
Vector3d VertexMeshDrawer<M>::renderingVertexNormal(const Index& id) const
{
    const Index& mappedId = vVertexMap[id];
    return Vector3d(this->vRenderingVertexNormals[mappedId*3], this->vRenderingVertexNormals[mappedId*3+1], this->vRenderingVertexNormals[mappedId*3+2]);
}

template<class M>
Color VertexMeshDrawer<M>::renderingVertexColor(const Index& id) const
{
    const Index& mappedId = vVertexMap[id];
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

    if (this->vMesh == nullptr) {
        return;
    }

    vVertexMap.resize(this->vMesh->nextVertexId(), NULL_ID);

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
        if (this->vMesh->isVertexDeleted(vId))
            continue;

        resetRenderingVertex(vId);
    }
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertexNormals()
{
    typedef typename M::VertexId VertexId;

    if (this->vMesh->hasVertexNormals()) {
        this->vRenderingVertexNormals.resize(this->vMesh->vertexNumber() * 3);

        #pragma omp parallel for
        for (VertexId vId = 0; vId < this->vMesh->nextVertexId(); ++vId) {
            if (this->vMesh->isVertexDeleted(vId))
                continue;

            resetRenderingVertexNormal(vId);
        }
    }
    else {
        this->vRenderingVertexNormals.clear();
    }
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertexColors()
{
    typedef typename M::VertexId VertexId;

    if (this->vMesh->hasVertexColors()) {
        this->vRenderingVertexColors.resize(this->vMesh->vertexNumber() * 4);

        #pragma omp parallel for
        for (VertexId vId = 0; vId < this->vMesh->nextVertexId(); ++vId) {
            if (this->vMesh->isVertexDeleted(vId))
                continue;

            resetRenderingVertexColor(vId);
        }
    }
    else {
        this->vRenderingVertexColors.clear();
    }
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertex(const Index& id)
{
    setRenderingVertex(id, this->vMesh->vertexPoint(id));
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertexNormal(const Index& id)
{
    typedef typename M::VertexNormal VertexNormal;
    const VertexNormal& normal = this->vMesh->vertexNormal(id);
    setRenderingVertexNormal(id, normal);
}

template<class M>
void VertexMeshDrawer<M>::resetRenderingVertexColor(const Index& id)
{
    typedef typename M::VertexColor VertexColor;
    const VertexColor& color = this->vMesh->vertexColor(id);
    setRenderingVertexColor(id, color);
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
    const Index& mappedId = vVertexMap[id];
    this->vRenderingVertices[mappedId*3] = p.x();
    this->vRenderingVertices[mappedId*3+1] = p.y();
    this->vRenderingVertices[mappedId*3+2] = p.z();
}

template<class M>
void VertexMeshDrawer<M>::setRenderingVertexNormal(const Index& id, const Vector3d& n)
{
    const Index& mappedId = vVertexMap[id];
    this->vRenderingVertexNormals[mappedId*3] = n.x();
    this->vRenderingVertexNormals[mappedId*3+1] = n.y();
    this->vRenderingVertexNormals[mappedId*3+2] = n.z();
}

template<class M>
void VertexMeshDrawer<M>::setRenderingVertexColor(const Index& id, const Color &c)
{
    const Index& mappedId = vVertexMap[id];
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

    glPointSize(this->vertexSize());

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, this->vRenderingVertices.data());

    if (this->vertexColorMode() == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
        if (!this->vRenderingVertexColors.empty()) {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, 0, this->vRenderingVertexColors.data());
        }
        else {
            glDisableClientState(GL_COLOR_ARRAY);
            glColor(this->vDefaultVertexColor);
        }
    }
    else {
        assert(this->vertexColorMode() == VertexMeshDrawerBase::VERTEX_COLOR_UNIFORM);
        glColor(this->vertexUniformColor());
    }

    glDrawArrays(GL_POINTS, 0, this->vRenderingVertices.size() / 3);

    if (this->vertexColorMode() == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
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
    const double vertexSize = this->vertexSize() * (this->boundingBox().diagonal().norm() * bboxFactor);

    for (Index i = 0; i < this->vRenderingVertices.size()/3; i++) {
        Point point(this->vRenderingVertices[i*3], this->vRenderingVertices[i*3+1], this->vRenderingVertices[i*3+2]);

        if (this->vertexColorMode() == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
            Color color;

            if (!this->vRenderingVertexColors.empty()) {
                color = Color(this->vRenderingVertexColors[i*4], this->vRenderingVertexColors[i*4+1], this->vRenderingVertexColors[i*4+2], this->vRenderingVertexColors[i*4+3]);
            }
            else {
                color = vDefaultVertexColor;
            }

            drawSphere(point, vertexSize, color, subdivision, subdivision);
        }
        else {
            assert(this->vertexColorMode() == VertexMeshDrawerBase::VERTEX_COLOR_UNIFORM);
            drawSphere(point, vertexSize, this->vertexUniformColor(), subdivision, subdivision);
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

    if (this->vRenderingVertexNormals.empty())
        return;

    const int subdivision = 5;
    const double arrowRadius = getVertexNormalRadius();
    const double arrowLength = getVertexNormalLength();

    for (Index i = 0; i < this->vRenderingVertices.size()/3; i++) {
        Point point(this->vRenderingVertices[i*3], this->vRenderingVertices[i*3+1], this->vRenderingVertices[i*3+2]);
        VertexNormal normal(this->vRenderingVertexNormals[i*3], this->vRenderingVertexNormals[i*3+1], this->vRenderingVertexNormals[i*3+2]);

        Point end = point + (normal  * arrowLength);

        if (this->vertexColorMode() == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
            Color color;

            if (!this->vRenderingVertexColors.empty()) {
                color = Color(this->vRenderingVertexColors[i*4], this->vRenderingVertexColors[i*4+1], this->vRenderingVertexColors[i*4+2], this->vRenderingVertexColors[i*4+3]);
            }
            else {
                color = vDefaultVertexColor;
            }

            drawArrow(point, end, arrowRadius, color, subdivision, subdivision);
        }
        else {
            assert(this->vertexColorMode() == VertexMeshDrawerBase::VERTEX_COLOR_UNIFORM);
            drawArrow(point, end, arrowRadius, this->vertexUniformColor(), subdivision, subdivision);
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

    glPointSize(this->vertexSize());

    glBegin(GL_POINTS);

    for (Index vId = 0; vId < this->vMesh->nextVertexId(); ++vId) {
        if (this->vMesh->isVertexDeleted(vId))
            continue;

        Point point = this->renderingVertex(vId);

        float alpha = this->vertexUniformColor().alphaF();
        if (this->vertexColorMode() == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
            if (!this->vRenderingVertexColors.empty()) {
                alpha = this->renderingVertexColor(vId).alphaF();
            }
            else {
                alpha = vDefaultVertexColor.alphaF();
            }
        }
        if (this->vertexTransparency() && alpha <= EPSILON) {
            continue;
        }

        Canvas::PickingData pickingData;
        pickingData.identifier = Canvas::PICKING_MESH_VERTEX;

        pickingData.addValue(drawableId);
        pickingData.addValue(vId);

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

        float alpha = this->vertexUniformColor().alphaF();
        if (this->vertexColorMode() == VertexMeshDrawerBase::VERTEX_COLOR_PER_VERTEX) {
            if (!this->vRenderingVertexColors.empty()) {
                alpha = this->renderingVertexColor(vId).alphaF();
            }
            else {
                alpha = vDefaultVertexColor.alphaF();
            }
        }
        if (this->vertexTransparency() && alpha <= EPSILON) {
            continue;
        }

        Canvas::PickingData pickingData;
        pickingData.identifier = Canvas::PICKING_MESH_VERTEX;

        pickingData.addValue(drawableId);
        pickingData.addValue(vId);

        pickingNameMap.push_back(pickingData);

        glPushName(pickingNameMap.size() - 1);
        drawSphere(point, vertexSize, this->vertexUniformColor(), subdivision, subdivision);
        glPopName();
    }

    glEnd();
}

template<class M>
double VertexMeshDrawer<M>::getVertexSize() const
{
    const double bboxFactor = 0.001;
    return this->vertexSize() * (this->boundingBox().diagonal().norm() * bboxFactor);
}

template<class M>
double VertexMeshDrawer<M>::getVertexNormalRadius() const
{
    const double radiusBBoxFactor = 0.0002;
    return this->boundingBox().diagonal().norm() * radiusBBoxFactor * this->vertexNormalSize();
}

template<class M>
double VertexMeshDrawer<M>::getVertexNormalLength() const
{
    const double lengthBBoxFactor = 0.005;
    return this->boundingBox().diagonal().norm() * lengthBBoxFactor * this->vertexNormalSize();
}

}

#endif
