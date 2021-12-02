#include "animation.h"

namespace nvl {

template<class T>
Animation<T>::Animation()
{

}

template<class T>
void Animation<T>::clear()
{
    vKeyframes.clear();
}

template<class T>
Size Animation<T>::keyframeNumber() const
{
    return vKeyframes.size();
}

template<class T>
typename Animation<T>::FrameId Animation<T>::addKeyframe(const double& time, const std::vector<T>& transformations)
{
    vKeyframes.push_back(AnimationFrame<T>(time, transformations));
    return vKeyframes.size() - 1;
}

template<class T>
typename Animation<T>::FrameId Animation<T>::addKeyframe(AnimationFrame<T>& frame)
{
    vKeyframes.push_back(frame);
    return vKeyframes.size() - 1;
}

template<class T>
void Animation<T>::setKeyframes(const std::vector<AnimationFrame<T>>& frames)
{
    vKeyframes = frames;
}

template<class T>
void Animation<T>::setKeyframe(const FrameId& id, AnimationFrame<T>& frame)
{
    vKeyframes[id] = frame;
}

template<class T>
const std::vector<AnimationFrame<T>>& Animation<T>::keyframes() const
{
    return vKeyframes;
}

template<class T>
std::vector<AnimationFrame<T>>& Animation<T>::keyframes()
{
    return vKeyframes;
}

template<class T>
const AnimationFrame<T>& Animation<T>::keyframe(const FrameId& id) const
{
    return vKeyframes[id];
}

template<class T>
AnimationFrame<T>& Animation<T>::keyframe(const FrameId& id)
{
    return vKeyframes[id];
}

template<class T>
double Animation<T>::duration() const
{
    if (vKeyframes.empty())
        return 0.0;

    return vKeyframes[vKeyframes.size() - 1].time();
}

template<class T>
const std::string& Animation<T>::name() const
{
    return vName;
}

template<class T>
void Animation<T>::setName(const std::string &value)
{
    vName = value;
}

}
