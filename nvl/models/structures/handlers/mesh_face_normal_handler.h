#ifndef NVL_MODELS_MESH_FACE_NORMAL_HANDLER_H
#define NVL_MODELS_MESH_FACE_NORMAL_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/containers/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class F, class N>
class MeshFaceNormalHandler
{

public:

    /* Typedefs */

    typedef F Face;
    typedef N Normal;

    typedef typename F::Id FaceId;

    typedef VectorWithDelete<Normal> Container;


    /* Constructors */

    MeshFaceNormalHandler();


    /* Methods */

    Normal& faceNormal(const FaceId& id);
    const Normal& faceNormal(const FaceId& id) const;
    void setFaceNormal(const FaceId& id, const Normal& normal);

    Normal& faceNormal(const Face& face);
    const Normal& faceNormal(const Face& face) const;
    void setFaceNormal(const Face& face, const Normal& normal);


protected:

    Container vFaceNormals;

};

}

#include "mesh_face_normal_handler.cpp"

#endif // NVL_MODELS_MESH_FACE_NORMAL_HANDLER_H
