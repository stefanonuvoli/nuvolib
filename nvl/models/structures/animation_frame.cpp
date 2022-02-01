/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "animation_frame.h"

namespace nvl {

template<class T>
AnimationFrame<T>::AnimationFrame() : vTime(0)
{

}

template<class T>
AnimationFrame<T>::AnimationFrame(double time, const std::vector<T>& transformations) :
    vTime(time), vTransformations(transformations)
{

}

template<class T>
void AnimationFrame<T>::clear()
{
    vTime = 0;
    vTransformations.clear();
}

template<class T>
double AnimationFrame<T>::time() const
{
    return vTime;
}

template<class T>
void AnimationFrame<T>::setTime(const double& value)
{
    vTime = value;
}

template<class T>
const T& AnimationFrame<T>::transformation(const Index& jId) const
{
    return vTransformations[jId];
}

template<class T>
T& AnimationFrame<T>::transformation(const Index& jId)
{
    return vTransformations[jId];
}

template<class T>
const std::vector<T>& AnimationFrame<T>::transformations() const
{
    return vTransformations;
}

template<class T>
std::vector<T>& AnimationFrame<T>::transformations()
{
    return vTransformations;
}

template<class T>
void AnimationFrame<T>::setTransformations(const std::vector<T>& transformations)
{
    vTransformations = transformations;
}

}
