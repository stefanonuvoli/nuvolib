#ifndef NVL_MODELS_MESH_FACE_MATERIAL_HANDLER_H
#define NVL_MODELS_MESH_FACE_MATERIAL_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class F, class M>
class MeshFaceMaterialHandler
{

public:

    /* Typedefs */

    typedef F Face;
    typedef M Material;

    typedef typename F::Id FaceId;
    typedef typename M::Id MaterialId;

    typedef VectorWithDelete<MaterialId> Container;


    /* Constructors */

    MeshFaceMaterialHandler();


    /* Methods */

    bool faceMaterialIsNull(const FaceId& id) const;
    MaterialId& faceMaterial(const FaceId& id);
    const MaterialId& faceMaterial(const FaceId& id) const;
    void setFaceMaterial(const FaceId& id, const MaterialId& materialId);
    void setFaceMaterial(const FaceId& id, const Material& material);
    void unsetFaceMaterial(const FaceId& id);

    bool faceMaterialIsNull(const Face& face) const;
    MaterialId& faceMaterial(const Face& face);
    const MaterialId& faceMaterial(const Face& face) const;
    void setFaceMaterial(const Face& face, const MaterialId& materialId);
    void setFaceMaterial(const Face& face, const Material& material);
    void unsetFaceMaterial(const Face& face);


protected:

    Container vFaceMaterials;

};

}

#include "mesh_face_material_handler.cpp"

#endif // NVL_MODELS_MESH_FACE_MATERIAL_HANDLER_H
