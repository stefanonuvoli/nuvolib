#include "model_drawer_base.h"

namespace nvl {

NVL_INLINE ModelDrawerBase::ModelDrawerBase() :
    vAnimationSpeed(1.0),
    vAnimationLoop(false),
    vAnimationSkinningMode(SKINNING_DUAL_QUATERNIONS),
    vAnimationBlend(true),
    vAnimationKeepKeyframes(true),
    vAnimationTargetFPS(30)
{

}

NVL_INLINE double ModelDrawerBase::animationSpeed() const
{
    return vAnimationSpeed;
}

NVL_INLINE void ModelDrawerBase::setAnimationSpeed(double value)
{
    vAnimationSpeed = value;
}

NVL_INLINE bool ModelDrawerBase::animationLoop() const
{
    return vAnimationLoop;
}

NVL_INLINE void ModelDrawerBase::setAnimationLoop(bool value)
{
    vAnimationLoop = value;
}

NVL_INLINE typename ModelDrawerBase::SkinningMode ModelDrawerBase::animationSkinningMode() const
{
    return vAnimationSkinningMode;
}

NVL_INLINE void ModelDrawerBase::setAnimationSkinningMode(SkinningMode value)
{
    vAnimationSkinningMode = value;
}

NVL_INLINE void ModelDrawerBase::setAnimationTargetFPS(double fps)
{
    vAnimationTargetFPS = fps;
}

NVL_INLINE double ModelDrawerBase::animationTargetFPS()
{
    return vAnimationTargetFPS;
}

NVL_INLINE void ModelDrawerBase::setAnimationBlend(bool blend)
{
    vAnimationBlend = blend;
}

NVL_INLINE bool ModelDrawerBase::animationBlend()
{
    return vAnimationBlend;
}

NVL_INLINE void ModelDrawerBase::setAnimationKeepKeyframes(bool keep)
{
    vAnimationKeepKeyframes = keep;
}

NVL_INLINE bool ModelDrawerBase::animationKeepKeyframes()
{
    return vAnimationKeepKeyframes;
}

}
