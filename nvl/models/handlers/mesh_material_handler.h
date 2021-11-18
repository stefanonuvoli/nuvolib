#ifndef NVL_MODELS_MESH_MATERIAL_HANDLER_H
#define NVL_MODELS_MESH_MATERIAL_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class M>
class MeshMaterialHandler
{

public:

    /* Typedefs */

    typedef M Material;
    typedef Index MaterialId;

    typedef VectorWithDelete<Material> Container;


    /* Constructors */

    MeshMaterialHandler();


    /* Materials */

    Size materialNumber() const;
    MaterialId nextMaterialId() const;

    Material& material(const MaterialId& id);
    const Material& material(const MaterialId& id) const;

    MaterialId addMaterial(const Material& material);

    MaterialId allocateMaterials(const Size& n);
    MaterialId allocateMaterials(const Size& n, const Material& Material);

    void deleteMaterial(const MaterialId& id);
    bool isMaterialDeleted(const MaterialId& id) const;

    void deleteMaterial(const Material& material);
    bool isMaterialDeleted(const Material& material) const;

    IteratorWrapper<Container, typename Container::iterator> materials();
    IteratorWrapper<const Container, typename Container::const_iterator> materials() const;

    void clearMaterials();



protected:

    Container vMaterials;

};

}

#include "mesh_material_handler.cpp"

#endif // NVL_MODELS_MESH_MATERIAL_HANDLER_H
