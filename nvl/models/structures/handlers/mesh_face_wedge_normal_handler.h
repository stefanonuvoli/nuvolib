#ifndef NVL_MODELS_MESH_FACE_WEDGE_NORMAL_HANDLER_H
#define NVL_MODELS_MESH_FACE_WEDGE_NORMAL_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/containers/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class F, class N>
class MeshFaceWedgeNormalHandler
{

public:

    /* Typedefs */

    typedef F Face;
    typedef N WedgeNormal;

    typedef typename F::Id FaceId;
    typedef Index WedgeNormalId;

    typedef VectorWithDelete<std::vector<WedgeNormalId>> Container;


    /* Constructors */

    MeshFaceWedgeNormalHandler();


    /* Methods */

    bool faceWedgeNormalsAreNull(const FaceId& id) const;
    std::vector<WedgeNormalId>& faceWedgeNormals(const FaceId& id);
    const std::vector<WedgeNormalId>& faceWedgeNormals(const FaceId& id) const;
    void setFaceWedgeNormals(const FaceId& id, const std::vector<WedgeNormalId>& wedgeNormalId);
    void unsetFaceWedgeNormals(const FaceId& id);

    bool faceWedgeNormalsAreNull(const Face& face) const;
    std::vector<WedgeNormalId>& faceWedgeNormals(const Face& face);
    const std::vector<WedgeNormalId>& faceWedgeNormals(const Face& face) const;
    void setFaceWedgeNormals(const Face& face, const std::vector<WedgeNormalId>& wedgeNormalId);
    void unsetFaceWedgeNormals(const Face& face);


protected:

    Container vFaceWedgeNormals;

};

}

#include "mesh_face_wedge_normal_handler.cpp"

#endif // NVL_MODELS_MESH_FACE_WEDGE_NORMAL_HANDLER_H
