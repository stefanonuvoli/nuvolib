#include "model.h"

namespace nvl {

template<class M, class S, class SW, class A>
Model<M,S,SW,A>::Model() :
    vName("Model")
{

}

template<class M, class S, class SW, class A>
void Model<M,S,SW,A>::initializeSkinningWeights()
{
    skinningWeights.initialize(mesh.nextVertexId(), skeleton.jointNumber());
}

template<class M, class S, class SW, class A>
void Model<M,S,SW,A>::resizeSkinningWeights()
{
    skinningWeights.resize(mesh.nextVertexId(), skeleton.jointNumber());
}

template<class M, class S, class SW, class A>
void Model<M,S,SW,A>::clear()
{
    mesh.clear();
    skeleton.clear();
    skinningWeights.clear();
    vAnimations.clear();
    vName = "Model";
}

template<class M, class S, class SW, class A>
Size Model<M,S,SW,A>::animationNumber() const
{
    return vAnimations.size();
}

template<class M, class S, class SW, class A>
typename Model<M,S,SW,A>::AnimationId Model<M,S,SW,A>::addAnimation(const Model::Animation& animation)
{
    vAnimations.push_back(animation);
    return vAnimations.size() - 1;
}

template<class M, class S, class SW, class A>
typename Model<M,S,SW,A>::Animation& Model<M,S,SW,A>::animation(const AnimationId& id)
{
    return vAnimations[id];
}

template<class M, class S, class SW, class A>
const typename Model<M,S,SW,A>::Animation& Model<M,S,SW,A>::animation(const AnimationId& id) const
{
    return vAnimations[id];
}

template<class M, class S, class SW, class A>
void Model<M,S,SW,A>::setAnimation(const Model::AnimationId &id, const Model::Animation &animation)
{
     vAnimations[id] = animation;
}

template<class M, class S, class SW, class A>
const std::vector<A>& Model<M,S,SW,A>::animations() const
{
    return vAnimations;
}

template<class M, class S, class SW, class A>
void Model<M,S,SW,A>::removeAnimation(const Model::AnimationId &id)
{
    vAnimations.erase(vAnimations.begin() + id);
}

template<class M, class S, class SW, class A>
const std::string& Model<M,S,SW,A>::name() const
{
    return vName;
}

template<class M, class S, class SW, class A>
void Model<M,S,SW,A>::setName(const std::string& value)
{
    vName = value;
}

}
