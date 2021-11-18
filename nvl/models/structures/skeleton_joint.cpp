#include "skeleton_joint.h"

namespace nvl {

template<class T>
SkeletonJoint<T>::SkeletonJoint() : SkeletonJoint(T::Identity(), "")
{
    assert(true);
}

template<class T>
SkeletonJoint<T>::SkeletonJoint(const T& restPose) : SkeletonJoint(restPose, "")
{

}

template<class T>
SkeletonJoint<T>::SkeletonJoint(const T& restPose, const std::string& name) :
    vId(NULL_ID),
    vRestPose(restPose),
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
const T& SkeletonJoint<T>::restPose() const
{
    return vRestPose;
}

template<class T>
T& SkeletonJoint<T>::restPose()
{
    return vRestPose;
}

template<class T>
void SkeletonJoint<T>::setRestPose(const T& restPose)
{
    vRestPose = restPose;
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
