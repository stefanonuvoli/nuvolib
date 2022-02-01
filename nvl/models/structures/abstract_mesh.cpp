/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "abstract_mesh.h"

namespace nvl {

NVL_INLINE AbstractMesh::AbstractMesh() :
    vType(UNKNOWN)
{

}

NVL_INLINE MeshType AbstractMesh::type() const
{
    return vType;
}

}
