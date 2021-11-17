#include "mesh_material_handler.h"

namespace nvl {

template<class M>
MeshMaterialHandler<M>::MeshMaterialHandler()
{

}

template<class M>
Size MeshMaterialHandler<M>::materialNumber() const
{
    return vMaterials.size();
}

template<class M>
typename MeshMaterialHandler<M>::MaterialId MeshMaterialHandler<M>::nextMaterialId() const
{
    return vMaterials.realSize();
}

template<class M>
typename MeshMaterialHandler<M>::Material& MeshMaterialHandler<M>::material(const MaterialId& id)
{
    return vMaterials[id];
}

template<class M>
const typename MeshMaterialHandler<M>::Material& MeshMaterialHandler<M>::material(const MaterialId& id) const
{
    return vMaterials[id];
}

template<class M>
typename MeshMaterialHandler<M>::MaterialId MeshMaterialHandler<M>::addMaterial(const Material& material)
{
    MaterialId newId = nextMaterialId();
    vMaterials.push_back(material);
    vMaterials[newId].setId(newId);
    return newId;
}

template<class M>
typename MeshMaterialHandler<M>::MaterialId MeshMaterialHandler<M>::allocateMaterials(const Size& n)
{
    return allocateMaterials(n, Material());
}

template<class M>
typename MeshMaterialHandler<M>::MaterialId MeshMaterialHandler<M>::allocateMaterials(const Size& n, const Material& material)
{
    Index firstIndex = vMaterials.realSize();
    Index lastIndex = firstIndex + n;

    vMaterials.resize(lastIndex, material);
    for (Index i = firstIndex; i < lastIndex; i++) {
        vMaterials[i].setId(i);
    }

    return firstIndex;
}

template<class M>
void MeshMaterialHandler<M>::deleteMaterial(const MaterialId& id)
{
    Size currentId = id;
    vMaterials[id].setId(NULL_ID);
    vMaterials.erase(currentId);
}

template<class M>
bool MeshMaterialHandler<M>::isMaterialDeleted(const MaterialId& id) const
{
    return vMaterials.isDeleted(id);
}

template<class M>
void MeshMaterialHandler<M>::deleteMaterial(const Material& material)
{
    deleteMaterial(material.id());
}

template<class M>
bool MeshMaterialHandler<M>::isMaterialDeleted(const Material& material) const
{
    return isMaterialDeleted(material.id());
}

template<class M>
IteratorWrapper<typename MeshMaterialHandler<M>::Container, typename MeshMaterialHandler<M>::Container::iterator> MeshMaterialHandler<M>::materials()
{
    return IteratorWrapper<Container, typename Container::iterator>(&vMaterials);
}

template<class M>
IteratorWrapper<const typename MeshMaterialHandler<M>::Container, typename MeshMaterialHandler<M>::Container::const_iterator> MeshMaterialHandler<M>::materials() const
{
    return IteratorWrapper<const Container, typename Container::const_iterator>(&vMaterials);
}

template<class M>
void MeshMaterialHandler<M>::clearMaterials()
{
    vMaterials.clear();
}

}
