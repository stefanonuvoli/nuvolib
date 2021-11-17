#include "mesh_face_material_handler.h"

namespace nvl {

template<class F, class M>
MeshFaceMaterialHandler<F,M>::MeshFaceMaterialHandler()
{

}

template<class F, class M>
bool MeshFaceMaterialHandler<F,M>::faceMaterialIsNull(const FaceId& id) const
{
    assert(id < vFaceMaterials.realSize() && "Face materials not enabled or not properly initialized.");

    return vFaceMaterials[id] == NULL_ID;
}

template<class F, class M>
typename MeshFaceMaterialHandler<F,M>::MaterialId& MeshFaceMaterialHandler<F,M>::faceMaterialId(const FaceId& id)
{
    assert(id < vFaceMaterials.realSize() && "Face materials not enabled or not properly initialized.");

    return vFaceMaterials[id];
}

template<class F, class M>
const typename MeshFaceMaterialHandler<F,M>::MaterialId& MeshFaceMaterialHandler<F,M>::faceMaterialId(const FaceId& id) const
{
    assert(id < vFaceMaterials.realSize() && "Face materials not enabled or not properly initialized.");

    return vFaceMaterials[id];
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setFaceMaterialId(const FaceId& id, const MaterialId& materialId)
{
    assert(id < vFaceMaterials.realSize() && "Face materials not enabled or not properly initialized.");

    vFaceMaterials[id] = materialId;
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setFaceMaterialId(const FaceId& id, const Material& material)
{
    setFaceMaterialId(id, material.id());
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::unsetFaceMaterialId(const FaceId& id)
{
    assert(id < vFaceMaterials.realSize() && "Face materials not enabled or not properly initialized.");

    vFaceMaterials[id] = NULL_ID;
}

template<class F, class M>
bool MeshFaceMaterialHandler<F,M>::faceMaterialIsNull(const Face& face) const
{
    return faceMaterialIsNull(face.id());
}

template<class F, class M>
typename MeshFaceMaterialHandler<F,M>::MaterialId& MeshFaceMaterialHandler<F,M>::faceMaterialId(const Face& face)
{
    return faceMaterialId(face.id());
}

template<class F, class M>
const typename MeshFaceMaterialHandler<F,M>::MaterialId& MeshFaceMaterialHandler<F,M>::faceMaterialId(const Face& face) const
{
    return faceMaterialId(face.id());
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setFaceMaterialId(const Face& face, const MaterialId& materialId)
{
    setFaceMaterialId(face.id(), materialId);
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setFaceMaterialId(const Face& face, const Material& material)
{
    setFaceMaterialId(face.id(), material.id());
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::unsetFaceMaterialId(const Face& face)
{
    unsetFaceMaterialId(face.id());
}

}
