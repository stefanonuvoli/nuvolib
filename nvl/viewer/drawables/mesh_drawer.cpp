#include "mesh_drawer.h"

#include <nvl/math/point.h>

#include <nvl/models/mesh_geometric_information.h>

namespace nvl {

template<class M>
MeshDrawer<M>::MeshDrawer() : MeshDrawer(nullptr)
{

}

template<class M>
MeshDrawer<M>::MeshDrawer(M* mesh, const bool visible, const bool pickable) :
    MeshDrawerBase(),
    Drawable(visible),
    Pickable(pickable),
    GLFrameable(),
    vMesh(mesh),
    vBoundingBox(AlignedBox3d())
{
    update();
}

template<class M>
Point3d MeshDrawer<M>::sceneCenter() const
{
    if (this->vMesh == nullptr || vBoundingBox.isNull())
        return Point3d(0,0,0);

    return this->vFrame * vBoundingBox.center();
}

template<class M>
double MeshDrawer<M>::sceneRadius() const
{
    if (this->vMesh == nullptr || vBoundingBox.isNull()) {
        return 1.0;
    }

    Point3d min = this->vFrame * vBoundingBox.min();
    Point3d max = this->vFrame * vBoundingBox.max();

    Vector3d vec = max - min;

    return vec.norm() / 2;
}

template<class M>
void MeshDrawer<M>::update()
{
    updateBoundingBox();
}

template<class M>
M* MeshDrawer<M>::mesh() const
{
    return vMesh;
}

template<class M>
void MeshDrawer<M>::setMesh(M* mesh)
{
    vMesh = mesh;
    update();
}

template<class M>
AlignedBox3d MeshDrawer<M>::boundingBox() const
{
    return vBoundingBox;
}

template<class M>
void MeshDrawer<M>::setBoundingBox(const AlignedBox3d& boundingBox)
{
    vBoundingBox = boundingBox;
}

template<class M>
void MeshDrawer<M>::updateBoundingBox()
{
    if (this->vMesh == nullptr)
        return;

    setBoundingBox(nvl::meshBoundingBox(*vMesh));
}

}
