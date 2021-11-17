#ifndef NVL_MODELS_MESH_VECTOR_FACE_H
#define NVL_MODELS_MESH_VECTOR_FACE_H

#include <nvl/nuvolib.h>

#include <nvl/models/mesh_face.h>

#include <vector>
#include <ostream>

namespace nvl {

template<class V, class N, class M>
class MeshVectorFace : public MeshFace<V,N,M,std::vector<typename V::Id>>
{

public:

    /* Typedefs */

    typedef typename MeshFace<V,N,M,std::vector<typename V::Id>>::Id Id;
    typedef typename MeshFace<V,N,M,std::vector<typename V::Id>>::Vertex Vertex;
    typedef typename MeshFace<V,N,M,std::vector<typename V::Id>>::VertexId VertexId;
    typedef typename MeshFace<V,N,M,std::vector<typename V::Id>>::Normal Normal;
    typedef typename MeshFace<V,N,M,std::vector<typename V::Id>>::VertexNormal VertexNormal;
    typedef typename MeshFace<V,N,M,std::vector<typename V::Id>>::Material Material;
    typedef typename MeshFace<V,N,M,std::vector<typename V::Id>>::VertexContainer VertexContainer;


    /* Constructors */

    explicit MeshVectorFace();
    template<typename... Ts>
    explicit MeshVectorFace(Ts... vertexIds);


    /* Methods */

    template<class T, typename... Ts>
    void setVertexIds(const T& vertex, Ts... vertexIds);
    void setVertexIds(const std::vector<VertexId>& vector);

    void resizeVertexNumber(Size vertexNumber);

    void insertVertex(VertexId vId);
    void insertVertex(const Index& pos, VertexId vId);
    void insertVertex(const Vertex& vId);
    void insertVertex(const Index& pos, const Vertex& vertex);

    void eraseLastVertex();
    void eraseVertex(const Index& pos);
    void eraseVertex(const Vertex& vertex);

private:

    template<class T, typename... Ts>
    void setVertexIdsVariadicHelper(const Index& pos, const T& vertex, Ts... vertexIds);
    void setVertexIdsVariadicBase(const Index& pos, const Vertex& vertex);
    void setVertexIdsVariadicBase(const Index& pos, const VertexId& vertexId);
    void setVertexIdsVariadicHelper(const Index& pos);
};

template<class V, class N, class M>
std::ostream& operator<<(std::ostream& output, const MeshVectorFace<V,N,M>& face);

}

#include "mesh_vector_face.cpp"

#endif // NVL_MODELS_MESH_VECTOR_FACE_H
