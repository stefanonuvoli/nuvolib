#include "skeleton_joint.h"

namespace nvl {

template<class T>
SkeletonJoint<T>::SkeletonJoint() : SkeletonJoint(T::Identity(), "")
{
    assert(true);
}

template<class T>
SkeletonJoint<T>::SkeletonJoint(const T& bindPose) : SkeletonJoint(bindPose, "")
{

}

template<class T>
SkeletonJoint<T>::SkeletonJoint(const T& bindPose, const std::string& name) :
    vId(NULL_ID),
    vBindPose(bindPose),
    vName(name)
{

}

template<class T>
const std::string& SkeletonJoint<T>::name() const
{
    return vName;
}

template<class T>
void SkeletonJoint<T>::setName(const std::string& name)
{
    vName = name;
}

template<class T>
const T& SkeletonJoint<T>::bindPose() const
{
    return vBindPose;
}

template<class T>
T& SkeletonJoint<T>::bindPose()
{
    return vBindPose;
}

template<class T>
void SkeletonJoint<T>::setBindPose(const T& bindPose)
{
    vBindPose = bindPose;
}

template<class T>
const typename SkeletonJoint<T>::Id& SkeletonJoint<T>::id() const
{
    return vId;
}

template<class T>
void SkeletonJoint<T>::setId(const Id& value)
{
    vId = value;
}

}
