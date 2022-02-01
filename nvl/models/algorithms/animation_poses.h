/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_ANIMATION_POSES_H
#define NVL_MODELS_ANIMATION_POSES_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

/* ----------------------- DEFORMATION ----------------------- */

template<class S, class A>
void animationDeformationFromGlobal(
        const S& skeleton,
        A& animation);

template<class S, class F>
void animationFrameDeformationFromGlobal(
        const S& skeleton,
        std::vector<F>& frames);

template<class S, class F>
void animationFrameDeformationFromGlobal(
        const S& skeleton,
        F& frame);

template<class S, class A>
void animationDeformationFromLocal(
        const S& skeleton,
        A& animation);

template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        std::vector<F>& frames);

template<class S, class F>
void animationFrameDeformationFromLocal(
        const S& skeleton,
        F& frame);


/* ----------------------- LOCAL FROM GLOBAL ----------------------- */

template<class S, class A>
void animationLocalFromGlobal(
        const S& skeleton,
        A& animation);

template<class S, class F>
void animationFrameLocalFromGlobal(
        const S& skeleton,
        std::vector<F>& frames);
template<class S, class F>
void animationFrameLocalFromGlobal(
        const S& skeleton,
        F& frame);


/* ----------------------- GLOBAL FROM LOCAL ----------------------- */

template<class S, class A>
void animationGlobalFromLocal(
        const S& skeleton,
        A& animation);

template<class S, class F>
void animationFrameGlobalFromLocal(
        const S& skeleton,
        std::vector<F>& frames);
template<class S, class F>
void animationFrameGlobalFromLocal(
        const S& skeleton,
        F& frame);

}

#include "animation_poses.cpp"

#endif // NVL_MODELS_ANIMATION_POSES_H
