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
void animationFChangeDuration(std::vector<F>& frames, const double& duration);

}

#include "animation_transformations.cpp"

#endif // NVL_MODELS_ANIMATION_TRANSFORMATIONS_H
