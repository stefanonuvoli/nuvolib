#include "polyline_mesh_drawer.h"

#include <nvl/viewer/gl/draw_primitives.h>
#include <nvl/viewer/gl/draw_gl.h>

namespace nvl {

template<class M>
PolylineMeshDrawer<M>::PolylineMeshDrawer() : PolylineMeshDrawer<M>(nullptr)
{

}

template<class M>
PolylineMeshDrawer<M>::PolylineMeshDrawer(M* mesh, const bool visible, const bool pickable) :
    PolylineMeshDrawerBase(),
    VertexMeshDrawer<M>(mesh, visible, pickable)
{
    resetRenderingPolylineData();
    this->setVertexVisible(false);
}

template<class M>
void PolylineMeshDrawer<M>::draw() const
{
    if (this->vMesh == nullptr)
        return;

    VertexMeshDrawer<M>::draw();

    if (this->vPolylineVisible) {
        if (this->vPolylineShapeMode == PolylineMeshDrawerBase::POLYLINE_SHAPE_LINE) {
            drawPolylinesLines();
        }
        else if (this->vPolylineShapeMode == PolylineMeshDrawerBase::POLYLINE_SHAPE_CYLINDER) {
            drawPolylinesCylinder();
        }
        else {
            assert(this->vPolylineShapeMode == PolylineMeshDrawerBase::POLYLINE_SHAPE_ARROW);
            drawPolylinesArrow();
        }
    }
}

template<class M>
void PolylineMeshDrawer<M>::drawWithNames(Canvas* canvas, const Index drawableId) const
{
    if (this->vMesh == nullptr || !this->isPickable())
        return;

    VertexMeshDrawer<M>::drawWithNames(canvas, drawableId);

    if (this->vPolylineVisible) {
        if (this->vPolylineShapeMode == PolylineMeshDrawerBase::POLYLINE_SHAPE_LINE) {
            drawPolylinesLinesWithNames(canvas, drawableId);
        }
        else if (this->vPolylineShapeMode == PolylineMeshDrawerBase::POLYLINE_SHAPE_CYLINDER) {
            drawPolylinesCylinderWithNames(canvas, drawableId);
        }
        else {
            assert(this->vPolylineShapeMode == PolylineMeshDrawerBase::POLYLINE_SHAPE_ARROW);
            drawPolylinesArrowWithNames(canvas, drawableId);
        }
    }
}

template<class M>
void PolylineMeshDrawer<M>::update()
{
    VertexMeshDrawer<M>::update();

    resetRenderingPolylineData();
}

template<class M>
const std::vector<unsigned int>& PolylineMeshDrawer<M>::renderingPolyline(const Index &id) const
{
    Index mappedId = vPolylineMap[id];
    return this->vRenderingPolylines[mappedId];
}

template<class M>
Color PolylineMeshDrawer<M>::renderingPolylineColor(const Index& id) const
{
    Index mappedId = vPolylineMap[id];
    return Color(this->vRenderingPolylineColors[mappedId*3], this->vRenderingPolylineColors[mappedId*3+1], this->vRenderingPolylineColors[mappedId*3+2], this->vRenderingPolylineColors[mappedId*3+3]);
}

template<class M>
void PolylineMeshDrawer<M>::resetRenderingData()
{
    VertexMeshDrawer<M>::resetRenderingData();
    resetRenderingPolylineData();
}

template<class M>
void PolylineMeshDrawer<M>::resetRenderingPolylineData()
{
    typedef typename M::Polyline Polyline;

    if (this->vMesh == nullptr)
        return;

    vPolylineMap.resize(this->vMesh->nextPolylineId(), MAX_INDEX);

    Index i = 0;
    for (const Polyline& polyline : this->vMesh->polylines()) {
        vPolylineMap[polyline.id()] = i;
        ++i;
    }

    resetRenderingPolylines();
    resetRenderingPolylineColors();
}

template<class M>
void PolylineMeshDrawer<M>::resetRenderingPolylines()
{
    typedef typename M::PolylineId PolylineId;

    this->vRenderingPolylines.resize(this->vMesh->polylineNumber(), std::vector<unsigned int>());

    #pragma omp parallel for
    for (PolylineId pId = 0; pId < this->vMesh->nextPolylineId(); ++pId) {
        if (!this->vMesh->isPolylineDeleted(pId)) {
            resetRenderingPolyline(pId);
        }
    }
}

template<class M>
void PolylineMeshDrawer<M>::resetRenderingPolylineColors()
{
    typedef typename M::PolylineId PolylineId;

    this->vRenderingPolylineColors.resize(this->vMesh->polylineNumber() * 4);

    #pragma omp parallel for
    for (PolylineId pId = 0; pId < this->vMesh->nextPolylineId(); ++pId) {
        if (!this->vMesh->isPolylineDeleted(pId)) {
            resetRenderingPolylineColor(pId);
        }
    }
}

template<class M>
void PolylineMeshDrawer<M>::setRenderingPolylines(const std::vector<std::vector<unsigned int>>& renderingPolylines)
{
    this->vRenderingPolylines = renderingPolylines;
}

template<class M>
void PolylineMeshDrawer<M>::resetRenderingPolylineColor(const Index& id)
{
    typedef typename M::Polyline Polyline;
    const Polyline& polyline = this->vMesh->polyline(id);
    setRenderingPolylineColor(id, polyline.color());
}

template<class M>
void PolylineMeshDrawer<M>::setRenderingPolyline(const Index& id, const std::vector<unsigned int>& p)
{
    Index mappedId = vPolylineMap[id];
    this->vRenderingPolylines[mappedId] = p;
}

template<class M>
void PolylineMeshDrawer<M>::setRenderingPolylineColors(const std::vector<float>& renderingPolylineColors)
{
    this->vRenderingPolylineColors = renderingPolylineColors;
}

template<class M>
void PolylineMeshDrawer<M>::resetRenderingPolyline(const Index& id)
{
    typedef typename M::Polyline Polyline;

    const Polyline& polyline = this->vMesh->polyline(id);

    std::vector<unsigned int> vertexIndices(polyline.vertexNumber());

    #pragma omp parallel for
    for (Index j = 0; j < polyline.vertexNumber(); ++j) {
        vertexIndices[j] = this->vVertexMap[polyline.vertexId(j)];
    }

    setRenderingPolyline(id, vertexIndices);
}

template<class M>
void PolylineMeshDrawer<M>::setRenderingPolylineColor(const Index& id, const Color& c)
{
    Index mappedId = vPolylineMap[id];
    this->vRenderingPolylineColors[mappedId*4] = c.redF();
    this->vRenderingPolylineColors[mappedId*4+1] = c.greenF();
    this->vRenderingPolylineColors[mappedId*4+2] = c.blueF();
    this->vRenderingPolylineColors[mappedId*4+3] = c.alphaF();
}

template<class M>
void PolylineMeshDrawer<M>::drawPolylinesLines() const
{
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDepthRange(0.0, 1.0);
    glDepthFunc(GL_LEQUAL);

    glLineWidth(this->vPolylineSize);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, this->vRenderingVertices.data());

    if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_VERTEX) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, this->vRenderingVertexColors.data());
    }
    else if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_UNIFORM) {
        glColor(this->vPolylineUniformColor);
    }

    for (Index i = 0; i < this->vRenderingPolylines.size(); ++i) {
        if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_POLYLINE) {
            glColor4f(
                    this->vRenderingPolylineColors[i*4],
                    this->vRenderingPolylineColors[i*4+1],
                    this->vRenderingPolylineColors[i*4+2],
                    this->vRenderingPolylineColors[i*4+3]);
        }

        glDrawElements(GL_LINE_STRIP, this->vRenderingPolylines[i].size(), GL_UNSIGNED_INT, this->vRenderingPolylines[i].data());
    }

    if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_VERTEX) {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    glDepthFunc(GL_LESS);
    glDepthRange(0.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

template<class M>
void PolylineMeshDrawer<M>::drawPolylinesCylinder() const
{
    typedef typename M::Point Point;

    const int subdivision = 20;
    const double polylineSize = getPolylineSize();

    for (Index i = 0; i < this->vRenderingPolylines.size(); ++i) {
        for (Index j = 0; j < this->vRenderingPolylines[i].size() - 1; ++j) {
            unsigned int vId1 = this->vRenderingPolylines[i][j];
            unsigned int vId2 = this->vRenderingPolylines[i][j+1];

            Point point1(this->vRenderingVertices[vId1*3], this->vRenderingVertices[vId1*3+1], this->vRenderingVertices[vId1*3+2]);
            Point point2(this->vRenderingVertices[vId2*3], this->vRenderingVertices[vId2*3+1], this->vRenderingVertices[vId2*3+2]);

            if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_POLYLINE) {
                Color color(this->vRenderingPolylineColors[i*4], this->vRenderingPolylineColors[i*4+1], this->vRenderingPolylineColors[i*4+2], this->vRenderingPolylineColors[i*4+3]);
                drawCylinder(point1, point2, polylineSize, polylineSize, color, subdivision, subdivision);
            }
            else {
                assert(this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_VERTEX || this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_UNIFORM);
                drawCylinder(point1, point2, polylineSize, polylineSize, this->vPolylineUniformColor, subdivision, subdivision);
            }
        }
    }
}

template<class M>
void PolylineMeshDrawer<M>::drawPolylinesArrow() const
{
    typedef typename M::Point Point;

    const int subdivision = 20;
    const double arrowRadius = getPolylineRadius();

    for (Index i = 0; i < this->vRenderingPolylines.size(); ++i) {
        for (Index j = 0; j < this->vRenderingPolylines[i].size() - 1; ++j) {

            unsigned int vId1 = this->vRenderingPolylines[i][j];
            unsigned int vId2 = this->vRenderingPolylines[i][j+1];

            Point point1(this->vRenderingVertices[vId1*3], this->vRenderingVertices[vId1*3+1], this->vRenderingVertices[vId1*3+2]);
            Point point2(this->vRenderingVertices[vId2*3], this->vRenderingVertices[vId2*3+1], this->vRenderingVertices[vId2*3+2]);

            if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_POLYLINE) {
                Color color(this->vRenderingPolylineColors[i*4], this->vRenderingPolylineColors[i*4+1], this->vRenderingPolylineColors[i*4+2], this->vRenderingPolylineColors[i*4+3]);
                drawArrow(point1, point2, arrowRadius, color, subdivision, subdivision);
            }
            else {
                assert(this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_VERTEX || this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_UNIFORM);
                drawArrow(point1, point2, arrowRadius, this->vPolylineUniformColor, subdivision, subdivision);
            }
        }
    }
}


template<class M>
void PolylineMeshDrawer<M>::drawPolylinesLinesWithNames(Canvas* canvas, const Index drawableId) const
{
    typedef typename M::Point Point;
    typedef typename M::VertexId VertexId;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDepthRange(0.0, 1.0);
    glDepthFunc(GL_LEQUAL);

    glLineWidth(this->vPolylineSize);

    glBegin(GL_LINES);

    glColor(this->vPolylineUniformColor);

    for (Index pId = 0; pId < this->vMesh->nextPolylineId(); ++pId) {
        if (this->vMesh->isPolylineDeleted(pId))
            continue;

        float alpha = this->vPolylineUniformColor.alphaF();
        if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_POLYLINE) {
            alpha = this->renderingPolylineColor(pId).alphaF();
        }
        else if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_VERTEX) {
            float avgAlpha = 0.0f;
            for (Index j = 0; j < this->renderingPolyline(pId).size(); ++j) {
                VertexId vId1 = this->renderingPolyline(pId).at(j);
                Color currentColor = this->renderingVertexColor(vId1);
                avgAlpha += currentColor.alphaF();
            }
            alpha = avgAlpha / this->renderingPolyline(pId).size();
        }
        if (this->polylineTransparency() && alpha <= nvl::EPSILON) {
            continue;
        }

        for (Index j = 0; j < this->renderingPolyline(pId).size() - 1; ++j) {
            VertexId vId1 = this->renderingPolyline(pId).at(j);
            VertexId vId2 = this->renderingPolyline(pId).at(j+1);

            Point point1(this->vRenderingVertices[vId1*3], this->vRenderingVertices[vId1*3+1], this->vRenderingVertices[vId1*3+2]);
            Point point2(this->vRenderingVertices[vId2*3], this->vRenderingVertices[vId2*3+1], this->vRenderingVertices[vId2*3+2]);

            Canvas::PickingData pickingData;
            pickingData.value1 = drawableId;
            pickingData.identifier = Canvas::PICKING_MESH_POLYLINE;
            pickingData.value2 = pId;
            pickingNameMap.push_back(pickingData);

            glPushName(pickingNameMap.size() - 1);
            glVertex(point1);
            glVertex(point2);
            glPopName();
        }
    }

    glEnd();

    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

template<class M>
void PolylineMeshDrawer<M>::drawPolylinesCylinderWithNames(Canvas* canvas, const Index drawableId) const
{
    typedef typename M::Point Point;
    typedef typename M::VertexId VertexId;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    const int subdivision = 20;
    const double polylineSize = getPolylineSize();

    for (Index pId = 0; pId < this->vMesh->nextPolylineId(); ++pId) {
        if (this->vMesh->isPolylineDeleted(pId))
            continue;

        float alpha = this->vPolylineUniformColor.alphaF();
        if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_POLYLINE) {
            alpha = this->renderingPolylineColor(pId).alphaF();
        }
        else if (this->vPolylineColorMode == PolylineMeshDrawerBase::POLYLINE_COLOR_PER_VERTEX) {
            float avgAlpha = 0.0f;
            for (Index j = 0; j < this->renderingPolyline(pId).size(); ++j) {
                VertexId vId = this->renderingPolyline(pId).at(j);
                Color currentColor = this->renderingVertexColor(vId);
                avgAlpha += currentColor.alphaF();
            }
            alpha = avgAlpha / this->renderingPolyline(pId).size();
        }
        if (this->polylineTransparency() && alpha <= nvl::EPSILON) {
            continue;
        }

        for (Index j = 0; j < this->renderingPolyline(pId).size() - 1; ++j) {
            VertexId vId1 = this->renderingPolyline(pId).at(j);
            VertexId vId2 = this->renderingPolyline(pId).at(j+1);

            Point point1(this->vRenderingVertices[vId1*3], this->vRenderingVertices[vId1*3+1], this->vRenderingVertices[vId1*3+2]);
            Point point2(this->vRenderingVertices[vId2*3], this->vRenderingVertices[vId2*3+1], this->vRenderingVertices[vId2*3+2]);

            Canvas::PickingData pickingData;
            pickingData.value1 = drawableId;
            pickingData.identifier = Canvas::PICKING_MESH_POLYLINE;
            pickingData.value2 = pId;
            pickingNameMap.push_back(pickingData);

            glPushName(pickingNameMap.size() - 1);
            drawCylinder(point1, point2, polylineSize, polylineSize, this->vPolylineUniformColor, subdivision, subdivision);
            glPopName();
        }
    }
}

template<class M>
void PolylineMeshDrawer<M>::drawPolylinesArrowWithNames(Canvas* canvas, const Index drawableId) const
{
    typedef typename M::Point Point;
    typedef typename M::VertexId VertexId;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    const int subdivision = 20;
    const double arrowRadius = getPolylineRadius();

    for (Index pId = 0; pId < this->vMesh->nextPolylineId(); ++pId) {
        if (this->vMesh->isPolylineDeleted(pId))
            continue;

        for (Index j = 0; j < this->renderingPolyline(pId).size() - 1; ++j) {
            VertexId vId1 = this->renderingPolyline(pId).at(j);
            VertexId vId2 = this->renderingPolyline(pId).at(j+1);

            Point point1(this->vRenderingVertices[vId1*3], this->vRenderingVertices[vId1*3+1], this->vRenderingVertices[vId1*3+2]);
            Point point2(this->vRenderingVertices[vId2*3], this->vRenderingVertices[vId2*3+1], this->vRenderingVertices[vId2*3+2]);

            Canvas::PickingData pickingData;
            pickingData.value1 = drawableId;
            pickingData.identifier = Canvas::PICKING_MESH_POLYLINE;
            pickingData.value2 = pId;
            pickingNameMap.push_back(pickingData);

            glPushName(pickingNameMap.size() - 1);
            drawArrow(point1, point2, arrowRadius, this->vPolylineUniformColor, subdivision, subdivision);
            glPopName();
        }
    }
}

template<class M>
double PolylineMeshDrawer<M>::getPolylineSize() const
{
    const double bboxFactor = 0.001;
    return this->vPolylineSize * (this->boundingBox().diagonal().norm() * bboxFactor);
}

template<class M>
double PolylineMeshDrawer<M>::getPolylineRadius()  const
{
    const double radiusBBoxFactor = 0.001;
    return this->boundingBox().diagonal().norm() * radiusBBoxFactor / 3;
}

}
