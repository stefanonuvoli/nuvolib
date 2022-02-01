/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "skeleton_drawer_base.h"

namespace nvl {

NVL_INLINE SkeletonDrawerBase::SkeletonDrawerBase() :
    vSkeletonVisible(true),
    vJointVisible(true),
    vJointSize(5), vJointColor(100,100,255), vRootColor(255,100,100),
    vBoneVisible(true),
    vBoneSize(5), vBoneColor(100,255,100),
    vTransparency(false)
{

}

NVL_INLINE bool SkeletonDrawerBase::skeletonVisible() const
{
    return vSkeletonVisible;
}

NVL_INLINE void SkeletonDrawerBase::setSkeletonVisible(bool value)
{
    vSkeletonVisible = value;
}

NVL_INLINE bool SkeletonDrawerBase::jointVisible() const
{
    return vJointVisible;
}

NVL_INLINE void SkeletonDrawerBase::setJointVisible(bool value)
{
    vJointVisible = value;
}

NVL_INLINE int SkeletonDrawerBase::jointSize() const
{
    return vJointSize;
}

NVL_INLINE void SkeletonDrawerBase::setJointSize(int size)
{
    vJointSize = size;
}

NVL_INLINE const Color& SkeletonDrawerBase::jointColor() const
{
    return vJointColor;
}

NVL_INLINE void SkeletonDrawerBase::setJointColor(const Color& color)
{
    vJointColor = color;
}

NVL_INLINE const Color& SkeletonDrawerBase::rootColor() const
{
    return vRootColor;
}

NVL_INLINE void SkeletonDrawerBase::setRootColor(const Color& color)
{
    vRootColor = color;
}

NVL_INLINE bool SkeletonDrawerBase::boneVisible() const
{
    return vBoneVisible;
}

NVL_INLINE void SkeletonDrawerBase::setBoneVisible(bool value)
{
    vBoneVisible = value;
}

NVL_INLINE int SkeletonDrawerBase::boneSize() const
{
    return vBoneSize;
}

NVL_INLINE void SkeletonDrawerBase::setBoneSize(int size)
{
    vBoneSize = size;
}

NVL_INLINE const Color& SkeletonDrawerBase::boneColor() const
{
    return vBoneColor;
}

NVL_INLINE void SkeletonDrawerBase::setBoneColor(const Color& color)
{
    vBoneColor = color;
}

NVL_INLINE bool SkeletonDrawerBase::transparency() const
{
    return vTransparency;
}

NVL_INLINE void SkeletonDrawerBase::setTransparency(bool value)
{
    vTransparency = value;
}

}
