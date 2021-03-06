/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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

template<class S, class A>
void animationRemoveRootMotion(const S& skeleton, std::vector<A>& animations);

template<class S, class A>
void animationRemoveRootMotion(const S& skeleton, A& animation);

template<class S, class F>
void animationFrameRemoveRootMotion(const S& skeleton, std::vector<F>& frames);

template<class S, class F>
void animationFrameRemoveRootMotion(const S& skeleton, F& frame);

}

#include "animation_clean.cpp"

#endif // NVL_MODELS_ANIMATION_CLEAN_H
