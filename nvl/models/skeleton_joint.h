#ifndef NVL_MODELS_SKELETON_JOINT_H
#define NVL_MODELS_SKELETON_JOINT_H

#include <nvl/nuvolib.h>

#include <vector>
#include <string>

namespace nvl {

template<class T>
class SkeletonJoint
{

public:

    typedef Index JointId;
    typedef T Transformation;
    typedef typename T::Scalar Scalar;

    SkeletonJoint();
    SkeletonJoint(const T& restPose);
    SkeletonJoint(const T& restPose, const std::string& name);

    const std::string& name() const;
    void setName(const std::string &value);

    const T& restPose() const;
    T& restPose();
    void setRestPose(const T& restPose);

    const JointId& id() const;
    void setId(const JointId& id);

protected:

    JointId vId;
    T vRestPose;
    std::string vName;
};

}

#include "skeleton_joint.cpp"

#endif // NVL_MODELS_SKELETON_JOINT_H
