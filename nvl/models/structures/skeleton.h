#ifndef NVL_MODELS_SKELETON_H
#define NVL_MODELS_SKELETON_H

#include <nvl/nuvolib.h>

#include <nvl/models/structures/skeleton_joint.h>

#include <nvl/math/point.h>

#include <nvl/utilities/iterator_wrapper.h>

#include <vector>

namespace nvl {

template<class T>
class Skeleton
{

public:

    typedef T Transformation;
    typedef typename T::Scalar Scalar;

    typedef SkeletonJoint<T> Joint;
    typedef typename Joint::Id JointId;

    Skeleton();

    void clear();

    Size jointNumber() const;
    Size rootNumber() const;
    Size childrenNumber(const JointId& id) const;
    Size childrenNumber(const Joint& joint) const;

    JointId addRoot(const T& transformation, const std::string& name = "");
    JointId addChild(const Joint& parent, const T& transformation, const std::string& name = "");
    JointId addChild(const JointId& parentId, const T& transformation, const std::string& name = "");

    JointId addRoot(const Joint& joint);
    JointId addChild(const Joint& parent, const Joint& joint);
    JointId addChild(const JointId& parentId, const Joint& joint);

    const std::vector<Joint>& joints() const;
    const std::vector<JointId>& roots() const;
    const std::vector<JointId>& children(const JointId& id) const;
    const std::vector<JointId>& children(const Joint& joint) const;

    const Joint& joint(const JointId& id) const;
    Joint& joint(const JointId& id);

    const JointId& root(const Index& rId) const;

    const JointId& child(const JointId& id, const Index& cId) const;

    const Joint& parent(const JointId& id) const;
    Joint& parent(const JointId& id);
    const Joint& parent(const Joint& joint) const;
    Joint& parent(const Joint& joint);
    const JointId& parentId(const JointId& id) const;
    const JointId& parentId(const Joint& joint) const;

    bool isRoot(const Index& id) const;
    bool isRoot(const Joint& joint) const;
    bool isLeaf(const Index& id) const;
    bool isLeaf(const Joint& joint) const;

    const Point3<Scalar>& originPoint() const;
    void setOriginPoint(const Point3<Scalar> &value);

    const std::string& jointName(const JointId& id) const;
    std::string& jointName(const JointId& id);
    void setJointName(const JointId& id, const std::string& name);

    const std::string& jointName(const Joint& joint) const;
    std::string& jointName(const Joint& joint);
    void setJointName(const Joint& joint, const std::string& name);

    const Transformation& jointBindPose(const JointId& id) const;
    Transformation& jointBindPose(const JointId& id);
    void setJointBindPose(const JointId& id, const Transformation& transformation);

    const Transformation& jointBindPose(const Joint& joint) const;
    Transformation& jointBindPose(const Joint& joint);
    void setJointBindPose(const Joint& joint, const Transformation& transformation);

    const bool& jointIsHidden(const JointId& id) const;
    bool& jointIsHidden(const JointId& id);
    void setJointHidden(const JointId& id, const bool& hidden);

    const bool& jointIsHidden(const Joint& joint) const;
    bool& jointIsHidden(const Joint& joint);
    void setJointHidden(const Joint& joint, const bool& hidden);

protected:

    std::vector<SkeletonJoint<T>> vJoints;

    std::vector<JointId> vRoots;

    std::vector<JointId> vParents;
    std::vector<std::vector<JointId>> vChildren;

    Point3<Scalar> vOriginPoint;

};

}

#include "skeleton.cpp"

#endif // NVL_MODELS_SKELETON_H
