/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_PRINCIPAL_CURVATURE_DRAWER_H
#define NVL_VIEWER_PRINCIPAL_CURVATURE_DRAWER_H

#include <nvl/nuvolib.h>

#ifdef NVL_OPENGL_LOADED

#include <nvl/viewer/interfaces/drawable.h>

#include <nvl/math/principal_curvatures.h>

#include <nvl/math/alignedbox.h>

#include <nvl/utilities/color.h>

namespace nvl {

template<class M>
class PrincipalCurvatureDrawer : public Drawable
{

public:

    PrincipalCurvatureDrawer();
    PrincipalCurvatureDrawer(M* mesh, std::vector<PrincipalCurvatures<typename M::Scalar>>* pc, const bool visible = true);

    void draw() const override;
    Point3d sceneCenter() const override;
    double sceneRadius() const override;

    M* mesh() const;
    void setMesh(M* mesh);

    std::vector<PrincipalCurvatures<typename M::Scalar>>* pc() const;
    void setPC(std::vector<PrincipalCurvatures<typename M::Scalar>>* pc);

    void updateBoundingBox();
    AlignedBox3d boundingBox() const;
    void setBoundingBox(const AlignedBox3d& boundingBox);

protected:

    M* vMesh;
    std::vector<PrincipalCurvatures<typename M::Scalar>>* vPC;

    AlignedBox3d vBoundingBox;
    typename M::Scalar vAvgEdge;

};

}

#endif

#include "principal_curvature_drawer.cpp"

#endif // NVL_VIEWER_PRINCIPAL_CURVATURE_DRAWER_H
