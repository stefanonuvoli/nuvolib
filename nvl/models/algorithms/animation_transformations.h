/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_ANIMATION_TRANSFORMATIONS_H
#define NVL_MODELS_ANIMATION_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

#include <nvl/math/transformations.h>

namespace nvl {

/* ----------------------- GEOMETRICAL TRANSFORMATIONS ----------------------- */

template<class A, class T>
void animationApplyTransformation(std::vector<A>& animations, const T& transformation);
template<class A, class T>
void animationApplyTransformation(A& animation, const T& transformation);
template<class F, class T>
void animationFrameApplyTransformation(std::vector<F>& frame, const T& transformation);
template<class F, class T>
void animationFrameApplyTransformation(F& frame, const T& transformation);

template<class A, class T>
void animationApplyTransformation(std::vector<A>& animations, const std::vector<T>& transformations);
template<class A, class T>
void animationApplyTransformation(A& animation, const std::vector<T>& transformations);
template<class F, class T>
void animationFrameApplyTransformation(std::vector<F>& frame, const std::vector<T>& transformations);
template<class F, class T>
void animationFrameApplyTransformation(F& frame, const std::vector<T>& transformations);


/* ----------------------- TIME TRANSFORMATIONS ----------------------- */

template<class A>
void animationChangeDuration(A& animation, const double& duration);

template<class F>
void animationFrameChangeDuration(std::vector<F>& frames, const double& duration);

template<class A>
void animationChangeSpeed(A& animation, const double& speed);

template<class F>
void animationFrameChangeSpeed(std::vector<F>& frames, const double& speed);

}

#include "animation_transformations.cpp"

#endif // NVL_MODELS_ANIMATION_TRANSFORMATIONS_H
