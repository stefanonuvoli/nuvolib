/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MODEL_H
#define NVL_MODELS_MODEL_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class M, class S, class W, class A>
struct Model
{  

public:

    typedef M Mesh;
    typedef S Skeleton;
    typedef W SkinningWeights;
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
    void removeAnimation(const AnimationId& id);
    void clearAnimations();

    M mesh;
    S skeleton;
    W skinningWeights;
    std::string name;
    std::vector<A> animations;

};

}

#include "model.cpp"

#endif // NVL_MODELS_MODEL_H
