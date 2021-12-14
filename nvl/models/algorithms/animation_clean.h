#ifndef NVL_MODELS_ANIMATION_CLEAN_H
#define NVL_MODELS_ANIMATION_CLEAN_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class S, class A>
void animationRemoveScaling(const S& skeleton, std::vector<A>& animations);

template<class S, class A>
void animationRemoveScaling(const S& skeleton, A& animation);

template<class S, class F>
void animationFrameRemoveScaling(const S& skeleton, std::vector<F>& frames);

template<class S, class F>
void animationFrameRemoveScaling(const S& skeleton, F& frame);

}

#include "animation_clean.cpp"

#endif // NVL_MODELS_ANIMATION_CLEAN_H
