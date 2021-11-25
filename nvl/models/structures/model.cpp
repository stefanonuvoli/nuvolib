#include "model.h"

namespace nvl {

template<class M, class S, class W, class A>
Model<M,S,W,A>::Model() :
    vName("Model")
{

}

template<class M, class S, class W, class A>
void Model<M,S,W,A>::initializeSkinningWeights()
{
    skinningWeights.initialize(mesh.nextVertexId(), skeleton.jointNumber());
}

template<class M, class S, class W, class A>
void Model<M,S,W,A>::resizeSkinningWeights()
{
    skinningWeights.resize(mesh.nextVertexId(), skeleton.jointNumber());
}

template<class M, class S, class W, class A>
void Model<M,S,W,A>::clear()
{
    mesh.clear();
    skeleton.clear();
    skinningWeights.clear();
    vAnimations.clear();
    vName = "Model";
}

template<class M, class S, class W, class A>
Size Model<M,S,W,A>::animationNumber() const
{
    return vAnimations.size();
}

template<class M, class S, class W, class A>
typename Model<M,S ,W,A>::AnimationId Model<M,S ,W,A>::addAnimation(const Model::Animation& animation)
{
    vAnimations.push_back(animation);
    return vAnimations.size() - 1;
}

template<class M, class S, class W, class A>
typename Model<M,S ,W,A>::Animation& Model<M,S ,W,A>::animation(const AnimationId& id)
{
    return vAnimations[id];
}

template<class M, class S, class W, class A>
const typename Model<M,S ,W,A>::Animation& Model<M,S ,W,A>::animation(const AnimationId& id) const
{
    return vAnimations[id];
}

template<class M, class S, class W, class A>
void Model<M,S ,W,A>::setAnimation(const Model::AnimationId &id, const Model::Animation &animation)
{
     vAnimations[id] = animation;
}

template<class M, class S, class W, class A>
const std::vector<A>& Model<M,S ,W,A>::animations() const
{
    return vAnimations;
}

template<class M, class S, class W, class A>
void Model<M,S ,W,A>::removeAnimation(const Model::AnimationId &id)
{
    vAnimations.erase(vAnimations.begin() + id);
}

template<class M, class S, class W, class A>
const std::string& Model<M,S ,W,A>::name() const
{
    return vName;
}

template<class M, class S, class W, class A>
void Model<M,S ,W,A>::setName(const std::string& value)
{
    vName = value;
}

}
