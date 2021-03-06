/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_MESH_DRAWER_H
#define NVL_VIEWER_MESH_DRAWER_H

#include <nvl/nuvolib.h>

#ifdef NVL_OPENGL_LOADED

#include <nvl/viewer/interfaces/drawable.h>
#include <nvl/viewer/interfaces/pickable.h>
#include <nvl/viewer/drawables/mesh_drawer_base.h>
#include <nvl/viewer/gl/gl_frameable.h>

#include <nvl/math/alignedbox.h>

namespace nvl {

template<class M>
class MeshDrawer : public MeshDrawerBase, public Drawable, public Pickable, public GLFrameable
{

public:

    MeshDrawer();
    MeshDrawer(M* mesh, const bool visible = true, const bool pickable = true);
    virtual ~MeshDrawer() = default;

    virtual Point3d sceneCenter() const override;
    virtual double sceneRadius() const override;

    virtual void update() override;

    M* mesh() const;
    void setMesh(M* mesh);

    AlignedBox3d boundingBox() const;
    void setBoundingBox(const AlignedBox3d& boundingBox);
    void updateBoundingBox();


protected:

    M* vMesh;
    AlignedBox3d vBoundingBox;

};

}

#endif

#include "mesh_drawer.cpp"

#endif // NVL_VIEWER_MESH_DRAWER_H
