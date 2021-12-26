#ifndef NVL_MODELS_MESH_FACE_WEDGE_UV_HANDLER_H
#define NVL_MODELS_MESH_FACE_WEDGE_UV_HANDLER_H

#include <nvl/nuvolib.h>

#include <vector>

#include <nvl/structures/containers/vector_with_delete.h>

#include <nvl/utilities/iterator_wrapper.h>

namespace nvl {

template<class F, class U>
class MeshFaceWedgeUVHandler
{

public:

    /* Typedefs */

    typedef F Face;
    typedef U WedgeUV;

    typedef typename F::Id FaceId;
    typedef Index WedgeUVId;

    typedef VectorWithDelete<std::vector<WedgeUVId>> Container;


    /* Constructors */

    MeshFaceWedgeUVHandler();


    /* Methods */

    bool faceWedgeUVsAreNull(const FaceId& id) const;
    std::vector<WedgeUVId>& faceWedgeUVs(const FaceId& id);
    const std::vector<WedgeUVId>& faceWedgeUVs(const FaceId& id) const;
    void setFaceWedgeUVs(const FaceId& id, const std::vector<WedgeUVId>& wedgeUVId);
    void unsetFaceWedgeUVs(const FaceId& id);

    bool faceWedgeUVsAreNull(const Face& face) const;
    std::vector<WedgeUVId>& faceWedgeUVs(const Face& face);
    const std::vector<WedgeUVId>& faceWedgeUVs(const Face& face) const;
    void setFaceWedgeUVs(const Face& face, const std::vector<WedgeUVId>& wedgeUVId);
    void unsetFaceWedgeUVs(const Face& face);


protected:

    Container vFaceWedgeUVs;

};

}

#include "mesh_face_wedge_uv_handler.cpp"

#endif // NVL_MODELS_MESH_FACE_WEDGE_UV_HANDLER_H
