#ifndef NVL_MODELS_ANIMATION_SPLIT_H
#define NVL_MODELS_ANIMATION_SPLIT_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class A>
A animationSubset(const A& animation, const std::string& name, const double& timeStart, const double& timeEnd);

template<class A>
A animationSubset(const A& animation, const std::string& name, const typedef A::FrameId& timeStart, const typedef A::FrameId& timeEnd);

}

#include "animation_split.cpp"

#endif // NVL_MODELS_ANIMATION_SPLIT_H
