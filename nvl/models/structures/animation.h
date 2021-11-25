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

    typedef T Transformation;
    typedef AnimationFrame<T> Frame;
    typedef Index FrameId;

    Animation();

    void clear();

    Size keyframeNumber() const;
    FrameId addKeyframe(double time, const std::vector<T>& transformations);
    FrameId addKeyframe(AnimationFrame<T>& frame);
    void setKeyframes(const std::vector<AnimationFrame<T>>& frames);
    void setKeyframe(const FrameId& id, AnimationFrame<T>& frame);
    const std::vector<AnimationFrame<T>>& keyframes() const;
    std::vector<AnimationFrame<T>>& keyframes();
    const AnimationFrame<T>& keyframe(const FrameId& id) const;
    AnimationFrame<T>& keyframe(const FrameId& id);

    const std::string& name() const;
    void setName(const std::string &value);

protected:

    std::vector<AnimationFrame<T>> vKeyframes;

    std::string vName;

};

}

#include "animation.cpp"

#endif // NVL_MODELS_ANIMATION_H
