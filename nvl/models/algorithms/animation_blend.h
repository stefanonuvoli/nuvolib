/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_ANIMATION_BLEND_H
#define NVL_MODELS_ANIMATION_BLEND_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class A>
void animationBlend(
        A& animation,
        const double& fps = 30,
        const double& speed = 1,
        const bool& keepKeyframes = true);

template<class F>
void animationFrameBlend(
        std::vector<F>& animationFrames,
        const double& fps = 30,
        const double& speed = 1,
        const bool& keepKeyframes = true);

}

#include "animation_blend.cpp"

#endif // NVL_MODELS_ANIMATION_BLEND_H
