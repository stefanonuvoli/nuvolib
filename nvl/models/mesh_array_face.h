#ifndef NVL_MODELS_MESH_ARRAY_FACE_H
#define NVL_MODELS_MESH_ARRAY_FACE_H

#include <nvl/nuvolib.h>

#include <nvl/models/mesh_face.h>

#include <array>
#include <vector>
#include <ostream>

namespace nvl {

template<class V, class N, class M, Size D>
class MeshArrayFace : public MeshFace<V,N,M,std::array<typename V::Id,D>>
{
    static_assert(sizeof(D) >= 3, "Faces must be composed of at least 3 vertices.");

public:

    /* Typedefs */

    typedef typename MeshFace<V,N,M,std::array<typename V::Id,D>>::Id Id;
    typedef typename MeshFace<V,N,M,std::array<typename V::Id,D>>::Vertex Vertex;
    typedef typename MeshFace<V,N,M,std::array<typename V::Id,D>>::VertexId VertexId;
    typedef typename MeshFace<V,N,M,std::array<typename V::Id,D>>::Normal Normal;
    typedef typename MeshFace<V,N,M,std::array<typename V::Id,D>>::VertexNormal VertexNormal;
    typedef typename MeshFace<V,N,M,std::array<typename V::Id,D>>::Material Material;
    typedef typename MeshFace<V,N,M,std::array<typename V::Id,D>>::VertexContainer VertexContainer;


    /* Constructors */

    explicit MeshArrayFace();
    template<typename... Ts>
    explicit MeshArrayFace(Ts... vertexIds);


    /* Methods */

    template<class T, typename... Ts>
    void setVertexIds(const T& vertex, Ts... vertexIds);
    void setVertexIds(const std::vector<VertexId>& vector);


private:

    template<class T, typename... Ts>
    void setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertexIds);
    void setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex);
    void setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId);
    void setVertexIdsVariadicHelper(const Index& pos);

};


template<class V, class N, class M, Size D>
std::ostream& operator<<(std::ostream& output, const MeshArrayFace<V,N,M,D>& face);

}

#include "mesh_array_face.cpp"

#endif // NVL_MODELS_MESH_ARRAY_FACE_H
