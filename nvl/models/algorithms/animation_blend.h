#ifndef NVL_MODELS_ANIMATION_BLEND_H
#define NVL_MODELS_ANIMATION_BLEND_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class F>
void animationBlendFrameTransformations(
        std::vector<F>& animationFrames,
        const double& fps = 60,
        const double& speed = 1,
        const bool& keepKeyframes = true);

}

#include "animation_blend.cpp"

#endif // NVL_MODELS_ANIMATION_BLEND_H
