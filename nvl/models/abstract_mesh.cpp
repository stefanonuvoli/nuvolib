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
