#ifndef NVL_VIEWER_MESH_DRAWER_BASE_H
#define NVL_VIEWER_MESH_DRAWER_BASE_H

#include <nvl/nuvolib.h>

namespace nvl {

class MeshDrawerBase
{

public:

    MeshDrawerBase();
    virtual ~MeshDrawerBase() = default;

    virtual void update() = 0;

};

}

#include "mesh_drawer_base.cpp"

#endif // NVL_VIEWER_MESH_DRAWER_BASE_H
