#include "skeleton_drawer.h"

#include <nvl/viewer/gl/gl_primitives.h>

#include <nvl/math/constants.h>

namespace nvl {

template<class S>
SkeletonDrawer<S>::SkeletonDrawer() : SkeletonDrawer(nullptr)
{

}

template<class S>
SkeletonDrawer<S>::SkeletonDrawer(S* skeleton, const bool visible, const bool pickable) :
    Drawable(visible),
    Pickable(pickable),
    GLFrameable(),
    SkeletonDrawerBase(),
    vSkeleton(skeleton),
    vBoundingBox(AlignedBox3d())
{
    update();
}

template<class S>
void SkeletonDrawer<S>::draw() const
{
    glDisable(GL_LIGHTING);

    if (this->transparency()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_GREATER, EPSILON);
        glEnable(GL_ALPHA_TEST);
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if (vSkeleton == nullptr || !this->vSkeletonVisible)
        return;

    const int subdivision = 10;
    const double sizeJointBBoxFactor = 0.001;
    const double jointSize = this->boundingBox().diagonal().norm() * sizeJointBBoxFactor * this->vJointSize;
    const double sizeBoneBBoxFactor = 0.0005;
    const double boneSize = this->boundingBox().diagonal().norm() * sizeBoneBBoxFactor * this->vBoneSize;

    if (this->vBoneVisible) {
        for (Index bId = 0; bId < this->vRenderingBones.size(); ++bId) {
            if (this->vRenderingBones[bId].size() < 2)
                continue;

            float alpha = this->vRenderingBoneColors[bId*4+3];
            if (this->transparency() && alpha <= EPSILON) {
                continue;
            }

            Color color(this->vRenderingBoneColors[bId*4], this->vRenderingBoneColors[bId*4+1], this->vRenderingBoneColors[bId*4+2], this->vRenderingBoneColors[bId*4+3]);

            unsigned int vId1 = this->vRenderingBones[bId][0];
            unsigned int vId2 = this->vRenderingBones[bId][1];

            Point3d point1(this->vRenderingJoints[vId1*3], this->vRenderingJoints[vId1*3+1], this->vRenderingJoints[vId1*3+2]);
            Point3d point2(this->vRenderingJoints[vId2*3], this->vRenderingJoints[vId2*3+1], this->vRenderingJoints[vId2*3+2]);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            drawCylinder(point1, point2, boneSize, boneSize, color, subdivision, subdivision);
        }
    }

    if (this->vJointVisible) {
        for (Index i = 0; i < this->vRenderingJoints.size()/3; ++i) {
            Point3d position(this->vRenderingJoints[i*3], this->vRenderingJoints[i*3+1], this->vRenderingJoints[i*3+2]);
            Color color(this->vRenderingJointColors[i*4], this->vRenderingJointColors[i*4+1], this->vRenderingJointColors[i*4+2], this->vRenderingJointColors[i*4+3]);

            float alpha = color.alphaF();
            if (this->transparency() && alpha <= EPSILON) {
                continue;
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            drawSphere(position, jointSize, color, subdivision, subdivision);
        }
    }

    if (this->transparency()) {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
    }

    glDisable(GL_CULL_FACE);
}

template<class S>
void SkeletonDrawer<S>::drawWithNames(Canvas* canvas, const Index drawableId) const
{
    if (vSkeleton == nullptr || !this->vSkeletonVisible || !this->isPickable())
        return;

    std::vector<Canvas::PickingData>& pickingNameMap = canvas->pickingDataPool();

    const int subdivision = 10;
    const double sizeJointBBoxFactor = 0.001;
    const double jointSize = this->boundingBox().diagonal().norm() * sizeJointBBoxFactor * this->vJointSize;
    const double sizeBoneBBoxFactor = 0.0005;
    const double boneSize = this->boundingBox().diagonal().norm() * sizeBoneBBoxFactor * this->vBoneSize;

    if (this->vBoneVisible) {
        for (Index bId = 0; bId < this->vRenderingBones.size(); ++bId) {
            if (this->vRenderingBones[bId].size() < 2)
                continue;

            float alpha = this->vRenderingBoneColors[bId*4+3];
            if (this->transparency() && alpha <= EPSILON) {
                continue;
            }

            Color color(this->vRenderingBoneColors[bId*4], this->vRenderingBoneColors[bId*4+1], this->vRenderingBoneColors[bId*4+2], this->vRenderingBoneColors[bId*4+3]);

            unsigned int jId1 = this->vRenderingBones[bId][0];
            unsigned int jId2 = this->vRenderingBones[bId][1];

            Point3d point1(this->vRenderingJoints[jId1*3], this->vRenderingJoints[jId1*3+1], this->vRenderingJoints[jId1*3+2]);
            Point3d point2(this->vRenderingJoints[jId2*3], this->vRenderingJoints[jId2*3+1], this->vRenderingJoints[jId2*3+2]);

            Canvas::PickingData pickingData;

            pickingData.identifier = Canvas::PICKING_SKELETON_BONE;

            pickingData.addValue(drawableId);
            pickingData.addValue(jId1);
            pickingData.addValue(jId2);

            pickingNameMap.push_back(pickingData);

            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glPushName(pickingNameMap.size() - 1);
            drawCylinder(point1, point2, boneSize, boneSize, color, subdivision, subdivision);
            glPopName();
        }
    }

    if (this->vJointVisible) {
        for (Index jId = 0; jId < vSkeleton->jointNumber(); ++jId) {
            if (!vSkeleton->jointIsHidden(jId)) {
                Point3d point = this->renderingJoint(jId);
                Color color = this->renderingJointColor(jId);

                float alpha = color.alphaF();
                if (this->transparency() && alpha <= EPSILON) {
                    continue;
                }

                Canvas::PickingData pickingData;
                pickingData.identifier = Canvas::PICKING_SKELETON_JOINT;

                pickingData.addValue(drawableId);
                pickingData.addValue(jId);

                pickingNameMap.push_back(pickingData);

                glDisable(GL_LIGHTING);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glEnable(GL_CULL_FACE);

                glPushName(pickingNameMap.size() - 1);
                glCullFace(GL_FRONT);
                drawSphere(point, jointSize, color, subdivision, subdivision);
                glCullFace(GL_BACK);
                drawSphere(point, jointSize, color, subdivision, subdivision);
                glPopName();

                glDisable(GL_CULL_FACE);
            }
        }
    }
}

template<class S>
Point3d SkeletonDrawer<S>::sceneCenter() const
{
    if (vSkeleton == nullptr || vBoundingBox.isNull())
        return Point3d(0,0,0);

    return this->vFrame * vBoundingBox.center();
}

template<class S>
double SkeletonDrawer<S>::sceneRadius() const
{
    if (vSkeleton == nullptr || vBoundingBox.isNull())
        return 1.0;

    Point3d min = this->vFrame * vBoundingBox.min();
    Point3d max = this->vFrame * vBoundingBox.max();

    Vector3d vec = max - min;

    return vec.norm() / 2;
}

template<class S>
void SkeletonDrawer<S>::update()
{
    updateBoundingBox();

    resetRenderingData();
}

template<class S>
S* SkeletonDrawer<S>::skeleton() const
{
    return vSkeleton;
}

template<class S>
void SkeletonDrawer<S>::setSkeleton(S* skeleton)
{
    vSkeleton = skeleton;
    update();
}

template<class S>
Point3d SkeletonDrawer<S>::renderingJoint(const Index& id) const
{
    const Index& mappedId = vJointMap[id];
    return Point3d(this->vRenderingJoints[mappedId*3], this->vRenderingJoints[mappedId*3+1], this->vRenderingJoints[mappedId*3+2]);
}

template<class S>
Color SkeletonDrawer<S>::renderingJointColor(const Index& id) const
{
    const Index& mappedId = vJointMap[id];
    return Color(this->vRenderingJointColors[mappedId*4], this->vRenderingJointColors[mappedId*4+1], this->vRenderingJointColors[mappedId*4+2], this->vRenderingJointColors[mappedId*4+3]);
}

template<class S>
Color SkeletonDrawer<S>::renderingBoneColor(const Index& id) const
{
    const Index& mappedId = vJointMap[id];
    return Color(this->vRenderingBoneColors[mappedId*4], this->vRenderingBoneColors[mappedId*4+1], this->vRenderingBoneColors[mappedId*4+2], this->vRenderingBoneColors[mappedId*4+3]);
}

template<class S>
void SkeletonDrawer<S>::resetRenderingData()
{
    typedef typename S::JointId JointId;

    if (vSkeleton == nullptr) {
        return;
    }

    vJointMap.resize(this->vSkeleton->jointNumber(), NULL_ID);

    vVisibleJointNumber = 0;
    for (JointId jId = 0; jId < this->vSkeleton->jointNumber(); ++jId) {
        if (!this->vSkeleton->jointIsHidden(jId)) {
            vJointMap[jId] = vVisibleJointNumber;
            vVisibleJointNumber++;
        }
    }

    resetRenderingJoints();
    resetRenderingJointColors();
    resetRenderingBones();
    resetRenderingBoneColors();
}

template<class S>
void SkeletonDrawer<S>::resetRenderingJoints()
{
    typedef typename S::JointId JointId;

    this->vRenderingJoints.resize(vVisibleJointNumber * 3);

    #pragma omp parallel for
    for (JointId jId = 0; jId < this->vSkeleton->jointNumber(); ++jId) {
        if (!this->vSkeleton->jointIsHidden(jId)) {
            resetRenderingJoint(jId);
        }
    }
}

template<class S>
void SkeletonDrawer<S>::resetRenderingJointColors()
{
    typedef typename S::JointId JointId;

    this->vRenderingJointColors.resize(vVisibleJointNumber * 4);

    #pragma omp parallel for
    for (JointId jId = 0; jId < this->vSkeleton->jointNumber(); ++jId) {
        if (!this->vSkeleton->jointIsHidden(jId)) {
            resetRenderingJointColor(jId);
        }
    }
}

template<class S>
void SkeletonDrawer<S>::resetRenderingBones()
{
    typedef typename S::JointId JointId;

    this->vRenderingBones.resize(vVisibleJointNumber);

    #pragma omp parallel for
    for (JointId jId = 0; jId < this->vSkeleton->jointNumber(); ++jId) {
        if (!this->vSkeleton->jointIsHidden(jId)) {
            resetRenderingBone(jId);
        }
    }
}

template<class S>
void SkeletonDrawer<S>::resetRenderingBoneColors()
{
    typedef typename S::JointId JointId;

    this->vRenderingBoneColors.resize(vVisibleJointNumber * 4);

    #pragma omp parallel for
    for (JointId jId = 0; jId < this->vSkeleton->jointNumber(); ++jId) {
        if (!this->vSkeleton->jointIsHidden(jId)) {
            resetRenderingBoneColor(jId);
        }
    }
}

template<class S>
void SkeletonDrawer<S>::resetRenderingJoint(const Index& id)
{
    Point3d p = this->vSkeleton->jointBindPose(id) * this->vSkeleton->originPoint();

    setRenderingJoint(id, p);
}

template<class S>
void SkeletonDrawer<S>::resetRenderingJointColor(const Index& id)
{
    typedef typename S::JointId JointId;

    const JointId& parentId = this->vSkeleton->parentId(id);

    if (parentId != NULL_ID) {
        setRenderingJointColor(id, this->vJointColor);
    }
    else {
        setRenderingJointColor(id, this->vRootColor);
    }
}

template<class S>
void SkeletonDrawer<S>::resetRenderingBone(const Index& id)
{
    typedef typename S::JointId JointId;

    const JointId& parentId = this->vSkeleton->parentId(id);

    if (parentId != NULL_ID && !this->vSkeleton->jointIsHidden(parentId)) {
        std::array<Index, 2> bone;
        bone[0] = parentId;
        bone[1] = id;

        this->setRenderingBone(id, bone);
    }
}

template<class S>
void SkeletonDrawer<S>::resetRenderingBoneColor(const Index& id)
{
    setRenderingBoneColor(id, this->vBoneColor);
}

template<class S>
void SkeletonDrawer<S>::setRenderingJoints(const std::vector<double>& renderingJoints)
{
    this->vRenderingJoints = renderingJoints;
}

template<class S>
void SkeletonDrawer<S>::setRenderingJointColors(const std::vector<float>& renderingJointColors)
{
    this->vRenderingJointColors = renderingJointColors;
}

template<class S>
void SkeletonDrawer<S>::setRenderingBones(const std::vector<std::vector<unsigned int>>& renderingBones)
{
    this->vRenderingBones = renderingBones;
}

template<class S>
void SkeletonDrawer<S>::setRenderingBoneColors(const std::vector<float>& renderingBoneColors)
{
    this->vRenderingBoneColors = renderingBoneColors;
}

template<class S>
void SkeletonDrawer<S>::setRenderingJoint(const Index& id, const Point3d& p)
{
    const Index& mappedId = vJointMap[id];
    this->vRenderingJoints[mappedId*3] = p.x();
    this->vRenderingJoints[mappedId*3+1] = p.y();
    this->vRenderingJoints[mappedId*3+2] = p.z();
}

template<class S>
void SkeletonDrawer<S>::setRenderingJointColor(const Index& id, const Color& c)
{
    const Index& mappedId = vJointMap[id];
    this->vRenderingJointColors[mappedId*4] = c.redF();
    this->vRenderingJointColors[mappedId*4+1] = c.greenF();
    this->vRenderingJointColors[mappedId*4+2] = c.blueF();
    this->vRenderingJointColors[mappedId*4+3] = c.alphaF();
}

template<class S>
void SkeletonDrawer<S>::setRenderingBone(const Index& id, const std::array<Index, 2>& bone)
{
    const Index& mappedId = vJointMap[id];
    const Index& boneMappedId1 = vJointMap[bone[0]];
    const Index& boneMappedId2 = vJointMap[bone[1]];
    std::array<unsigned int, 2> values = {
        static_cast<int>(boneMappedId1),
        static_cast<int>(boneMappedId2)
    };

    this->vRenderingBones[mappedId] = values;
}

template<class S>
void SkeletonDrawer<S>::setRenderingBoneColor(const Index& id, const Color& c)
{
    const Index& mappedId = vJointMap[id];
    this->vRenderingBoneColors[mappedId*4] = c.redF();
    this->vRenderingBoneColors[mappedId*4+1] = c.greenF();
    this->vRenderingBoneColors[mappedId*4+2] = c.blueF();
    this->vRenderingBoneColors[mappedId*4+3] = c.alphaF();
}

template<class S>
AlignedBox3d SkeletonDrawer<S>::boundingBox() const
{
    return vBoundingBox;
}

template<class S>
void SkeletonDrawer<S>::setBoundingBox(const AlignedBox3d& boundingBox)
{
    vBoundingBox = boundingBox;
}

template<class S>
void SkeletonDrawer<S>::updateBoundingBox()
{
    typedef typename S::Joint Joint;

    if (vSkeleton == nullptr)
        return;

    vBoundingBox.setNull();

    for (const Joint& joint : vSkeleton->joints()) {
        if (!joint.isHidden()) {
            Point3d p = joint.bindPose() * this->vSkeleton->originPoint();

            vBoundingBox.extend(p);
        }
    }
}

}
