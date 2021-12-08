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
    SkeletonJoint(const T& bindPose);
    SkeletonJoint(const T& bindPose, const std::string& name);


    /* Methods */

    const Id& id() const;
    Id& id();
    void setId(const Id& id);

    const std::string& name() const;
    std::string& name();
    void setName(const std::string& value);

    const T& bindPose() const;
    T& bindPose();
    void setBindPose(const T& bindPose);

    const bool& isHidden() const;
    bool& isHidden();
    void setHidden(const bool& value);


protected:

    Id vId;
    T vBindPose;
    std::string vName;
    bool vHidden;
};

}

#include "skeleton_joint.cpp"

#endif // NVL_MODELS_SKELETON_JOINT_H
