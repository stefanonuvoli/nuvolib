/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "animation_transformations.h"

#include <vector>

#include <nvl/math/comparisons.h>

namespace nvl {

/* ----------------------- GEOMETRICAL TRANSFORMATIONS ----------------------- */

/**
 * @brief Apply transformation to animations
 * @param animations Animations
 * @param transformation Transformation
 */
template<class A, class T>
void animationApplyTransformation(std::vector<A>& animations, const T& transformation)
{
    #pragma omp parallel for
    for (Index aId = 0; aId < animations.size(); ++aId) {
        animationApplyTransformation(animations[aId], transformation);
    }
}

/**
 * @brief Apply transformation to animation
 * @param animation Animation
 * @param transformation Transformation
 */
template<class A, class T>
void animationApplyTransformation(A& animation, const T& transformation)
{
    animationFrameApplyTransformation(animation.keyframes(), transformation);
}

/**
 * @brief Apply transformation to animation frames
 * @param frames Animation frames
 * @param transformation Transformation
 */
template<class F, class T>
void animationFrameApplyTransformation(std::vector<F>& frames, const T& transformation)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameApplyTransformation(frames[fId], transformation);
    }
}

/**
 * @brief Apply transformation to animation frame
 * @param frames Animation frame
 * @param transformation Transformation
 */
template<class F, class T>
void animationFrameApplyTransformation(F& frame, const T& transformation)
{
    typedef typename F::Transformation Transformation;

    std::vector<Transformation>& frameTransformations = frame.transformations();

    if (!frameTransformations.empty()) {
        #pragma omp parallel for
        for (Index jId = 0; jId < frameTransformations.size(); ++jId) {
            frameTransformations[jId] = transformation * frameTransformations[jId];
        }
    }
}

/**
 * @brief Apply transformation to animations
 * @param animations Animations
 * @param transformations Transformation for each skeleton joint
 */
template<class A, class T>
void animationApplyTransformation(std::vector<A>& animations, const std::vector<T>& transformations)
{
    #pragma omp parallel for
    for (Index aId = 0; aId < animations.size(); ++aId) {
        animationApplyTransformation(animations[aId], transformations);
    }
}

/**
 * @brief Apply transformation to animation
 * @param animation Animation
 * @param transformations Transformation for each skeleton joint
 */
template<class A, class T>
void animationApplyTransformation(A& animation, const std::vector<T>& transformations)
{
    animationFrameApplyTransformation(animation.keyframes(), transformations);
}

/**
 * @brief Apply transformation to animation frames
 * @param animations Animation frames
 * @param transformations Transformation for each skeleton joint
 */
template<class F, class T>
void animationFrameApplyTransformation(std::vector<F>& frames, const std::vector<T>& transformations)
{
    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        animationFrameApplyTransformation(frames[fId], transformations);
    }
}

/**
 * @brief Apply transformation to animation frame
 * @param animations Animation frame
 * @param transformations Transformation for each skeleton joint
 */
template<class F, class T>
void animationFrameApplyTransformation(F& frame, const std::vector<T>& transformations)
{
    typedef typename F::Transformation Transformation;

    std::vector<Transformation>& frameTransformations = frame.transformations();

    if (!frameTransformations.empty()) {
        #pragma omp parallel for
        for (Index jId = 0; jId < frameTransformations.size(); ++jId) {
            frameTransformations[jId] = transformations[jId] * frameTransformations[jId];
        }
    }
}



/* ----------------------- TIME TRANSFORMATIONS ----------------------- */

/**
 * @brief Change duration of an animation
 * @param animation Animation
 * @param duration New duration
 */
template<class A>
void animationChangeDuration(A& animation, const double& duration)
{
    animationChangeDuration(animation.keyframes(), duration);
}

/**
 * @brief Change duration of an animation
 * @param frames Animation frames
 * @param duration New duration
 */
template<class F>
void animationFrameChangeDuration(std::vector<F>& frames, const double& duration)
{
    if (frames.empty())
        return;

    const double oldDuration = frames[frames.size() - 1].time();
    const double speed = oldDuration / duration;

    animationFrameChangeSpeed(frames, speed);
}

/**
 * @brief Change duration of an animation given a speed value
 * @param animation Animation
 * @param speed Speed
 */
template<class A>
void animationChangeSpeed(A& animation, const double& speed)
{
    animationChangeSpeed(animation.keyframes(), speed);
}

/**
 * @brief Change duration of an animation given a speed value
 * @param frames Animation frames
 * @param speed Speed
 */
template<class F>
void animationFrameChangeSpeed(std::vector<F>& frames, const double& speed)
{
    if (frames.empty() || epsEqual(speed, 1.0))
        return;

    #pragma omp parallel for
    for (Index fId = 0; fId < frames.size(); ++fId) {
        F& frame = frames[fId];
        frame.setTime(frame.time() / speed);
    }
}

}
