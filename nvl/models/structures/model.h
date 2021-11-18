#ifndef NVL_MODELS_MODEL_H
#define NVL_MODELS_MODEL_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class M, class S, class SW, class A>
struct Model
{
    typedef M Mesh;
    typedef S Skeleton;
    typedef SW SkinningWeights;
    typedef A Animation;
    typedef Index AnimationId;

    Model();

    void initializeSkinningWeights();
    void resizeSkinningWeights();

    void clear();

    Size animationNumber() const;
    AnimationId addAnimation(const Animation& animation);
    Animation& animation(const AnimationId& id);
    const Animation& animation(const AnimationId& id) const;
    void setAnimation(const AnimationId& id, const Animation& animation);
    const std::vector<A>& animations() const;
    void removeAnimation(const AnimationId& id);

    M mesh;
    S skeleton;
    SW skinningWeights;
    std::string vName;


public:
    const std::string& name() const;
    void setName(const std::string &value);

protected:

    std::vector<A> vAnimations;

};

}

#include "model.cpp"

#endif // NVL_MODELS_MODEL_H
