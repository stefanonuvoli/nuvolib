/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_ANIMATION_H
#define NVL_MODELS_ANIMATION_H

#include <nvl/nuvolib.h>

#include <nvl/models/structures/animation_frame.h>

#include <vector>

namespace nvl {

template<class T>
class Animation
{

public:

    typedef AnimationFrame<T> Frame;
    typedef Index FrameId;
    typedef typename Frame::Scalar Scalar;
    typedef typename Frame::Transformation Transformation;

    Animation();

    void clear();

    Size keyframeNumber() const;
    FrameId addKeyframe(const double& time, const std::vector<T>& transformations);
    FrameId addKeyframe(AnimationFrame<T>& frame);
    void setKeyframes(const std::vector<AnimationFrame<T>>& frames);
    void setKeyframe(const FrameId& id, AnimationFrame<T>& frame);
    const std::vector<AnimationFrame<T>>& keyframes() const;
    std::vector<AnimationFrame<T>>& keyframes();
    const AnimationFrame<T>& keyframe(const FrameId& id) const;
    AnimationFrame<T>& keyframe(const FrameId& id);

    double duration() const;

    const std::string& name() const;
    void setName(const std::string &value);

protected:

    std::vector<AnimationFrame<T>> vKeyframes;

    std::string vName;

};

}

#include "animation.cpp"

#endif // NVL_MODELS_ANIMATION_H
