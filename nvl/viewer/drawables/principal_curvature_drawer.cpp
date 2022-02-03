/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "principal_curvature_drawer.h"

#ifdef NVL_OPENGL_LOADED

#include <nvl/viewer/gl/gl_primitives.h>
#include <nvl/viewer/gl/gl_draw.h>

#include <nvl/math/constants.h>

#include <nvl/models/algorithms/mesh_geometric_information.h>

namespace nvl {

template<class M>
PrincipalCurvatureDrawer<M>::PrincipalCurvatureDrawer() : PrincipalCurvatureDrawer(nullptr, nullptr)
{

}

template<class M>
PrincipalCurvatureDrawer<M>::PrincipalCurvatureDrawer(M* mesh, std::vector<PrincipalCurvatures<typename M::Scalar>>* pc, const bool visible) :
    Drawable(visible),
    vMesh(mesh),
    vPC(pc),
    vBoundingBox(AlignedBox3d())
{
    updateBoundingBox();
}

template<class M>
void PrincipalCurvatureDrawer<M>::draw() const
{
    typedef typename M::VertexId VertexId;
    typedef typename M::Point Point;

    if (vMesh == nullptr || vPC == nullptr)
        return;

    glDisable(GL_LIGHTING);

    glDepthRange(0.0, 1.0);
    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (VertexId vId = 0; vId < vMesh->nextVertexId(); ++vId) {
        Point p = vMesh->vertexPoint(vId);

        Point p1 = p + (((*vPC)[vId].d1 * (*vPC)[vId].k1) / (30 * (vBoundingBox.diagonal().norm() / vAvgEdge)));
        Point p2 = p + (((*vPC)[vId].d2 * (*vPC)[vId].k2) / (30 * (vBoundingBox.diagonal().norm() / vAvgEdge)));

        Point midPoint1 = (p1 - p) * 0.5;
        Point midPoint2 = (p2 - p) * 0.5;

        drawLine(p - midPoint1, p1 - midPoint1, RED, 2);
        drawLine(p - midPoint2, p2 - midPoint2, BLUE, 2);
    }

    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_CULL_FACE);
}

template<class M>
Point3d PrincipalCurvatureDrawer<M>::sceneCenter() const
{
    if (vMesh == nullptr || vBoundingBox.isNull())
        return Point3d(0,0,0);

    return vBoundingBox.center();
}

template<class M>
double PrincipalCurvatureDrawer<M>::sceneRadius() const
{
    if (vMesh == nullptr || vBoundingBox.isNull())
        return 0.0;

    Point3d min = vBoundingBox.min();
    Point3d max = vBoundingBox.max();

    Vector3d vec = max - min;

    double radius = vec.norm() / 2;
    if (radius <= 0) {
        radius = EPSILON;
    }

    return radius;
}

template<class M>
M *PrincipalCurvatureDrawer<M>::mesh() const
{
    return vMesh;
}

template<class M>
void PrincipalCurvatureDrawer<M>::setMesh(M* mesh)
{
    vMesh = mesh;
    updateBoundingBox();
}

template<class M>
std::vector<PrincipalCurvatures<typename M::Scalar>>* PrincipalCurvatureDrawer<M>::pc() const
{
    return vPC;
}

template<class M>
void PrincipalCurvatureDrawer<M>::setPC(std::vector<PrincipalCurvatures<typename M::Scalar>>* pc)
{
    this->vPC = pc;
}

template<class M>
AlignedBox3d PrincipalCurvatureDrawer<M>::boundingBox() const
{
    return vBoundingBox;
}

template<class M>
void PrincipalCurvatureDrawer<M>::setBoundingBox(const AlignedBox3d& boundingBox)
{
    vBoundingBox = boundingBox;
}

template<class M>
void PrincipalCurvatureDrawer<M>::updateBoundingBox()
{
    if (vMesh == nullptr)
        return;

    vBoundingBox = meshBoundingBox(*vMesh);
    vAvgEdge = meshAverageEdgeLength(*vMesh);
}

}

#endif
