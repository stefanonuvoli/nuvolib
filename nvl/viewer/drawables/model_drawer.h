/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_MODEL_DRAWER_H
#define NVL_VIEWER_MODEL_DRAWER_H

#include <nvl/nuvolib.h>

#ifdef NVL_OPENGL_LOADED

#include <nvl/viewer/interfaces/drawable.h>
#include <nvl/viewer/interfaces/pickable.h>
#include <nvl/viewer/interfaces/animable.h>
#include <nvl/viewer/gl/gl_frameable.h>

#include <nvl/viewer/drawables/model_drawer_base.h>
#include <nvl/viewer/drawables/face_mesh_drawer.h>
#include <nvl/viewer/drawables/skeleton_drawer.h>

#include <nvl/models/animation_3d.h>

#include <nvl/math/dual_quaternion.h>

#include <chrono>

namespace nvl {

template<class M>
class ModelDrawer : public Drawable, public Pickable, public Animable, public GLFrameable, public ModelDrawerBase
{

public:

    typedef M Model;

    typedef typename FaceMeshDrawerBase::VertexShapeMode VertexShapeMode;
    typedef typename FaceMeshDrawerBase::VertexColorMode VertexColorMode;
    typedef typename FaceMeshDrawerBase::PolylineShapeMode PolylineShapeMode;
    typedef typename FaceMeshDrawerBase::PolylineColorMode PolylineColorMode;
    typedef typename FaceMeshDrawerBase::FaceShadingMode FaceShadingMode;
    typedef typename FaceMeshDrawerBase::FaceColorMode FaceColorMode;
    typedef typename Animation3d::Frame Frame;
    typedef ModelDrawerBase::SkinningMode SkinningMode;

    ModelDrawer();
    ModelDrawer(M* vModel, const bool visible = true, const bool pickable = true, const bool animable = true);

    void draw() const override;
    void drawWithNames(Canvas* canvas, const Index drawableId) const override;

    void startAnimation() override;
    void pauseAnimation() override;
    void stopAnimation() override;
    bool animate() override;
    bool animationRunning() const override;

    Point3d sceneCenter() const override;
    double sceneRadius() const override;

    virtual void resetRenderingData();
    void update() override;

    M* model() const;
    void setModel(M* model);

    FaceMeshDrawer<typename M::Mesh>& meshDrawer();
    SkeletonDrawer<typename M::Skeleton>& skeletonDrawer();

    void loadAnimation(const Index& id);
    void unloadAnimation();
    void reloadAnimation();
    bool isAnimationLoaded() const;
    Index loadedAnimation() const;

    void setCurrentFrameId(const Index& keyframeId);
    const Index& currentFrameId();
    const Frame& currentFrame() const;
    Frame& currentFrame();
    const std::vector<Frame>& animationFrames() const;
    std::vector<Frame>& animationFrames();
    void setAnimationFrames(const std::vector<Frame>& animationFrames);
    Size frameNumber();

    template<class T>
    void renderLinearBlendingSkinning(const std::vector<T>& transformations);
    void renderDualQuaternionSkinning(const std::vector<DualQuaterniond>& transformations);

    void updateBoundingBox();
    AlignedBox3d boundingBox() const;
    void setBoundingBox(const AlignedBox3d& boundingBox);

private:

    void resetAnimation();
    void processKeyframePose(const Index& frameId);


protected:

    M* vModel;
    AlignedBox3d vBoundingBox;

    FaceMeshDrawer<typename M::Mesh> vMeshDrawer;
    SkeletonDrawer<typename M::Skeleton> vSkeletonDrawer;

    Index vAnimationLoaded;

    std::vector<Frame> vAnimationFrames;
    std::vector<std::vector<DualQuaterniond>> dualQuaternionTransformations;

    Index vAnimationCurrentFrameId;

    bool vAnimationRunning;
    bool vAnimationPaused;
    std::chrono::time_point<std::chrono::system_clock> vAnimationStartTime;
    std::chrono::time_point<std::chrono::system_clock> vAnimationPauseTime;

};

}

#endif

#include "model_drawer.cpp"

#endif // NVL_VIEWER_MODEL_DRAWER_H
