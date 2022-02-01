/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
