#include "skeleton.h"

namespace nvl {

template<class T>
Skeleton<T>::Skeleton() : vOriginPoint(0,0,0)
{

}

template<class T>
void Skeleton<T>::clear()
{
    vJoints.clear();
    vParents.clear();
    vRoots.clear();
    vChildren.clear();
}

template<class T>
Size Skeleton<T>::jointNumber() const
{
    return vJoints.size();
}

template<class T>
Size Skeleton<T>::rootNumber() const
{
    return vRoots.size();
}

template<class T>
Size Skeleton<T>::childrenNumber(const JointId& id) const
{
    return vChildren[id].size();
}

template<class T>
Size Skeleton<T>::childrenNumber(const Joint& joint) const
{
    return childrenNumber(joint.id());
}

template<class T>
typename Skeleton<T>::JointId Skeleton<T>::addRoot(const T& transformation, const std::string& name)
{
    return addRoot(Joint(transformation, name));
}

template<class T>
typename Skeleton<T>::JointId Skeleton<T>::addChild(const Joint& parent, const T& transformation, const std::string& name)
{
    return addChild(parent, Joint(transformation, name));
}

template<class T>
typename Skeleton<T>::JointId Skeleton<T>::addChild(const JointId& parentId, const T& transformation, const std::string& name)
{
    return addChild(parentId, Joint(transformation, name));
}

template<class T>
typename Skeleton<T>::JointId Skeleton<T>::addRoot(const Joint& joint)
{
    JointId newId = vJoints.size();
    vJoints.push_back(joint);

    vJoints[newId].setId(newId);

    vRoots.push_back(newId);

    vParents.push_back(NULL_ID);

    vChildren.push_back(std::vector<JointId>());

    return newId;
}

template<class T>
typename Skeleton<T>::JointId Skeleton<T>::addChild(const Joint& parent, const Joint& joint)
{
    return addChild(parent.id(), joint);
}

template<class T>
typename Skeleton<T>::JointId Skeleton<T>::addChild(const JointId& parentId, const Joint& joint)
{
    JointId newId = vJoints.size();
    vJoints.push_back(joint);

    vJoints[newId].setId(newId);

    vParents.push_back(parentId);
    vChildren[parentId].push_back(newId);

    vChildren.push_back(std::vector<JointId>());

    return newId;
}

template<class T>
const std::vector<typename Skeleton<T>::Joint>& Skeleton<T>::joints() const
{
    return vJoints;
}

template<class T>
const std::vector<typename Skeleton<T>::JointId>& Skeleton<T>::roots() const
{
    return vRoots;
}

template<class T>
const std::vector<typename Skeleton<T>::JointId>& Skeleton<T>::children(const JointId& id) const
{
    return vChildren[id];
}

template<class T>
const std::vector<typename Skeleton<T>::JointId>& Skeleton<T>::children(const Skeleton::Joint& joint) const
{
    return vChildren[joint.id()];
}

template<class T>
const typename Skeleton<T>::Joint& Skeleton<T>::joint(const JointId& id) const
{
    return vJoints[id];
}

template<class T>
typename Skeleton<T>::Joint& Skeleton<T>::joint(const JointId& id)
{
    return vJoints[id];
}

template<class T>
const typename Skeleton<T>::JointId& Skeleton<T>::root(const Index& id) const
{
    return vRoots[id];
}

template<class T>
const typename Skeleton<T>::JointId& Skeleton<T>::child(const JointId& id, const Index& cId) const
{
    return vChildren[id][cId];
}

template<class T>
const typename Skeleton<T>::JointId& Skeleton<T>::parentId(const JointId& id) const
{
    return vParents[id];
}

template<class T>
const typename Skeleton<T>::JointId& Skeleton<T>::parentId(const Joint& joint) const
{
    return parentId(joint.id());
}

template<class T>
const typename Skeleton<T>::Joint& Skeleton<T>::parent(const JointId& id) const
{
    return vJoints[vParents[id]];
}

template<class T>
typename Skeleton<T>::Joint& Skeleton<T>::parent(const JointId& id)
{
    return vJoints[vParents[id]];
}

template<class T>
const typename Skeleton<T>::Joint& Skeleton<T>::parent(const Joint& joint) const
{
    return parent(joint.id());
}

template<class T>
typename Skeleton<T>::Joint& Skeleton<T>::parent(const Joint& joint)
{
    return parent(joint.id());
}

template<class T>
bool Skeleton<T>::isRoot(const Index& id) const
{
    return parentId(id) == NULL_ID;
}

template<class T>
bool Skeleton<T>::isRoot(const Joint& joint) const
{
    return parentId(joint) == NULL_ID;
}

template<class T>
bool Skeleton<T>::isLeaf(const Index& id) const
{
    return children(id).empty();
}

template<class T>
bool Skeleton<T>::isLeaf(const Joint& joint) const
{
    return children(joint).empty();
}

template<class T>
const Point3<typename Skeleton<T>::Scalar>& Skeleton<T>::originPoint() const
{
    return vOriginPoint;
}

template<class T>
void Skeleton<T>::setOriginPoint(const Point3<Scalar>& value)
{
    vOriginPoint = value;
}

template<class T>
const typename Skeleton<T>::Transformation& Skeleton<T>::jointBindPose(const JointId& id) const
{
    return joint(id).bindPose();
}

template<class T>
typename Skeleton<T>::Transformation& Skeleton<T>::jointBindPose(const JointId& id)
{
    return joint(id).bindPose();
}

template<class T>
void Skeleton<T>::setJointBindPose(const JointId& id, const Transformation& transformation)
{
     joint(id).setBindPose(transformation);
}

template<class T>
const typename Skeleton<T>::Transformation& Skeleton<T>::jointBindPose(const Joint& joint) const
{
    return jointBindPose(joint.id);
}

template<class T>
typename Skeleton<T>::Transformation& Skeleton<T>::jointBindPose(const Joint& joint)
{
    return jointBindPose(joint.id);
}

template<class T>
void Skeleton<T>::setJointBindPose(const Joint& joint, const Transformation& transformation)
{
    return setJointBindPose(joint.id, transformation);
}

}
