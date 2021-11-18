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

    /* Typedefs */

    typedef Index Id;

    typedef T Transformation;
    typedef typename T::Scalar Scalar;


    /* Constructors */

    SkeletonJoint();
    SkeletonJoint(const T& restPose);
    SkeletonJoint(const T& restPose, const std::string& name);


    /* Methods */

    const Id& id() const;
    void setId(const Id& id);

    const std::string& name() const;
    void setName(const std::string &value);

    const T& restPose() const;
    T& restPose();
    void setRestPose(const T& restPose);

protected:

    Id vId;
    T vRestPose;
    std::string vName;
};

}

#include "skeleton_joint.cpp"

#endif // NVL_MODELS_SKELETON_JOINT_H
