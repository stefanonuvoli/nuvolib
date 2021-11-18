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
typename MeshFaceMaterialHandler<F,M>::MaterialId& MeshFaceMaterialHandler<F,M>::faceMaterial(const FaceId& id)
{
    assert(id < vFaceMaterials.realSize() && "Face materials not enabled or not properly initialized.");

    return vFaceMaterials[id];
}

template<class F, class M>
const typename MeshFaceMaterialHandler<F,M>::MaterialId& MeshFaceMaterialHandler<F,M>::faceMaterial(const FaceId& id) const
{
    assert(id < vFaceMaterials.realSize() && "Face materials not enabled or not properly initialized.");

    return vFaceMaterials[id];
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setfaceMaterial(const FaceId& id, const MaterialId& materialId)
{
    assert(id < vFaceMaterials.realSize() && "Face materials not enabled or not properly initialized.");

    vFaceMaterials[id] = materialId;
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setfaceMaterial(const FaceId& id, const Material& material)
{
    setfaceMaterial(id, material.id());
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::unsetfaceMaterial(const FaceId& id)
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
typename MeshFaceMaterialHandler<F,M>::MaterialId& MeshFaceMaterialHandler<F,M>::faceMaterial(const Face& face)
{
    return faceMaterial(face.id());
}

template<class F, class M>
const typename MeshFaceMaterialHandler<F,M>::MaterialId& MeshFaceMaterialHandler<F,M>::faceMaterial(const Face& face) const
{
    return faceMaterial(face.id());
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setfaceMaterial(const Face& face, const MaterialId& materialId)
{
    setfaceMaterial(face.id(), materialId);
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setfaceMaterial(const Face& face, const Material& material)
{
    setfaceMaterial(face.id(), material.id());
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::unsetfaceMaterial(const Face& face)
{
    unsetfaceMaterial(face.id());
}

}
