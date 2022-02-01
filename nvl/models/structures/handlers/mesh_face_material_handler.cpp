/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
void MeshFaceMaterialHandler<F,M>::setFaceMaterial(const FaceId& id, const MaterialId& materialId)
{
    assert(id < vFaceMaterials.realSize() && "Face materials not enabled or not properly initialized.");

    vFaceMaterials[id] = materialId;
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setFaceMaterial(const FaceId& id, const Material& material)
{
    setFaceMaterial(id, material.id());
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::unsetFaceMaterial(const FaceId& id)
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
void MeshFaceMaterialHandler<F,M>::setFaceMaterial(const Face& face, const MaterialId& materialId)
{
    setFaceMaterial(face.id(), materialId);
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::setFaceMaterial(const Face& face, const Material& material)
{
    setFaceMaterial(face.id(), material.id());
}

template<class F, class M>
void MeshFaceMaterialHandler<F,M>::unsetFaceMaterial(const Face& face)
{
    unsetFaceMaterial(face.id());
}

}
