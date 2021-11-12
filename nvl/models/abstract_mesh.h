#ifndef NVL_MODELS_ABSTRACT_MESH_H
#define NVL_MODELS_ABSTRACT_MESH_H

#include <nvl/nuvolib.h>

namespace nvl {

enum MeshType { VERTEX, POLYLINE, FACE, EDGE, VOLUMETRIC, UNKNOWN };

class AbstractMesh
{

public:

    typedef MeshType Type;

    /* Constructors */

    AbstractMesh();
    virtual ~AbstractMesh() = default;

    MeshType type() const;

protected:

    MeshType vType;

};

}

#include "abstract_mesh.cpp"

#endif // NVL_MODELS_ABSTRACT_MESH_H
