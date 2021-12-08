#ifndef NVL_VIEWER_SKELETON_DRAWER_H
#define NVL_VIEWER_SKELETON_DRAWER_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/interfaces/drawable.h>
#include <nvl/viewer/interfaces/pickable.h>
#include <nvl/viewer/gl/glframeable.h>
#include <nvl/viewer/drawables/skeleton_drawer_base.h>

#include <nvl/math/alignedbox.h>

#include <nvl/utilities/color.h>

namespace nvl {

template<class S>
class SkeletonDrawer : public Drawable, public Pickable, public GLFrameable, public SkeletonDrawerBase
{

public:

    SkeletonDrawer();
    SkeletonDrawer(S* skeleton, const bool visible = true, const bool pickable = true);

    void draw() const override;
    void drawWithNames(Canvas* canvas, const Index drawableId) const override;

    Point3d sceneCenter() const override;
    double sceneRadius() const override;

    void update() override;

    S* skeleton() const;
    void setSkeleton(S* skeleton);

    Point3d renderingJoint(const Index& id) const;
    Color renderingJointColor(const Index& id) const;
    Color renderingBoneColor(const Index& id) const;

    void resetRenderingData();

    void resetRenderingJoints();
    void resetRenderingJointColors();
    void resetRenderingBones();
    void resetRenderingBoneColors();
    void resetRenderingJoint(const Index& id);
    void resetRenderingJointColor(const Index& id);
    void resetRenderingBone(const Index& id);
    void resetRenderingBoneColor(const Index& id);

    void setRenderingJoints(const std::vector<double>& renderingJoints);
    void setRenderingJointColors(const std::vector<float>& renderingJointColors);
    void setRenderingBones(const std::vector<std::vector<unsigned int>>& renderingBones);
    void setRenderingBoneColors(const std::vector<float>& renderingBoneColors);
    void setRenderingJoint(const Index& id, const Point3d& p);
    void setRenderingJointColor(const Index& id, const Color& c);
    void setRenderingBone(const Index& id, const std::array<Index, 2>& bone);
    void setRenderingBoneColor(const Index& id, const Color& c);

    void updateBoundingBox();
    AlignedBox3d boundingBox() const;
    void setBoundingBox(const AlignedBox3d& boundingBox);

protected:

    S* vSkeleton;
    AlignedBox3d vBoundingBox;

    std::vector<double> vRenderingJoints;
    std::vector<float> vRenderingJointColors;
    std::vector<std::array<unsigned int, 2>> vRenderingBones;
    std::vector<float> vRenderingBoneColors;

    Size vVisibleJointNumber;
    std::vector<Index> vJointMap;
};

}

#include "skeleton_drawer.cpp"

#endif // NVL_VIEWER_SKELETON_DRAWER_H
