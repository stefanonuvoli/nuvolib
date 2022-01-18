#include "model.h"

namespace nvl {

template<class M, class S, class W, class A>
Model<M,S,W,A>::Model() :
    name("Model")
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
    animations.clear();
    name = "Model";
}

template<class M, class S, class W, class A>
Size Model<M,S,W,A>::animationNumber() const
{
    return animations.size();
}

template<class M, class S, class W, class A>
typename Model<M,S ,W,A>::AnimationId Model<M,S ,W,A>::addAnimation(const Model::Animation& animation)
{
    animations.push_back(animation);
    return animations.size() - 1;
}

template<class M, class S, class W, class A>
typename Model<M,S ,W,A>::Animation& Model<M,S ,W,A>::animation(const AnimationId& id)
{
    return animations[id];
}

template<class M, class S, class W, class A>
const typename Model<M,S ,W,A>::Animation& Model<M,S ,W,A>::animation(const AnimationId& id) const
{
    return animations[id];
}

template<class M, class S, class W, class A>
void Model<M,S ,W,A>::setAnimation(const Model::AnimationId &id, const Model::Animation &animation)
{
     animations[id] = animation;
}

template<class M, class S, class W, class A>
void Model<M,S ,W,A>::clearAnimations()
{
    animations.clear();
}


template<class M, class S, class W, class A>
void Model<M,S ,W,A>::removeAnimation(const Model::AnimationId &id)
{
    animations.erase(animations.begin() + id);
}

}
