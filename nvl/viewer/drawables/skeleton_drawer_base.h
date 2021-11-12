#ifndef NVL_VIEWER_SKELETON_DRAWER_BASE_H
#define NVL_VIEWER_SKELETON_DRAWER_BASE_H

#include <nvl/nuvolib.h>

#include <nvl/utilities/color.h>

#include <nvl/math/alignedbox.h>

namespace nvl {

class SkeletonDrawerBase
{

public:

    SkeletonDrawerBase();
    virtual ~SkeletonDrawerBase() = default;

    bool skeletonVisible() const;
    void setSkeletonVisible(bool value);

    bool jointVisible() const;
    void setJointVisible(bool value);

    int jointSize() const;
    void setJointSize(int size);

    const Color& jointColor() const;
    void setJointColor(const Color& value);

    const Color& rootColor() const;
    void setRootColor(const Color& value);

    bool boneVisible() const;
    void setBoneVisible(bool value);

    int boneSize() const;
    void setBoneSize(int size);

    const Color& boneColor() const;
    void setBoneColor(const Color& value);

    bool transparency() const;
    void setTransparency(bool value);

    virtual void update() = 0;

protected:

    bool vSkeletonVisible;

    bool vJointVisible;
    int vJointSize;
    Color vJointColor;
    Color vRootColor;

    bool vBoneVisible;
    int vBoneSize;
    Color vBoneColor;

    bool vTransparency;

};

}

#include "skeleton_drawer_base.cpp"

#endif // NVL_VIEWER_SKELETON_DRAWER_BASE_H
