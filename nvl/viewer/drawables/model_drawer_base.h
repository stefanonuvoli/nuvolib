#ifndef NVL_VIEWER_MODEL_DRAWER_BASE_H
#define NVL_VIEWER_MODEL_DRAWER_BASE_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/drawables/face_mesh_drawer_base.h>

namespace nvl {

class ModelDrawerBase
{

public:

    enum SkinningMode { SKINNING_LINEAR_BLENDING, SKINNING_DUAL_QUATERNIONS };

    ModelDrawerBase();
    virtual ~ModelDrawerBase() = default;

    double animationSpeed() const;
    void setAnimationSpeed(double value);

    bool animationLoop() const;
    void setAnimationLoop(bool value);

    SkinningMode animationSkinningMode() const;
    void setAnimationSkinningMode(SkinningMode value);

    void setAnimationTargetFPS(double fps);
    double animationTargetFPS();

    void setAnimationBlend(bool blend);
    bool animationBlend();

    void setAnimationKeepKeyframes(bool keep);
    bool animationKeepKeyframes();

    virtual void update() = 0;

protected:

    double vAnimationSpeed;
    bool vAnimationLoop;
    SkinningMode vAnimationSkinningMode;
    bool vAnimationBlend;
    bool vAnimationKeepKeyframes;
    double vAnimationTargetFPS;

};

}

#include "model_drawer_base.cpp"

#endif // NVL_VIEWER_MODEL_DRAWER_BASE_H
