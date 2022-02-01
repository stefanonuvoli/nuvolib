/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_ANIMATION_FRAME_H
#define NVL_MODELS_ANIMATION_FRAME_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class T>
class AnimationFrame
{

public:

    typedef T Transformation;
    typedef typename Transformation::Scalar Scalar;

    AnimationFrame();
    AnimationFrame(double time, const std::vector<T>& transformations);

    void clear();

    double time() const;
    void setTime(const double& value);

    const T& transformation(const Index& jId) const;
    T& transformation(const Index& jId);

    const std::vector<T>& transformations() const;
    std::vector<T>& transformations();
    void setTransformations(const std::vector<T>& value);


protected:

    double vTime;
    std::vector<T> vTransformations;


};

}

#include "animation_frame.cpp"

#endif // NVL_MODELS_ANIMATION_FRAME_H
