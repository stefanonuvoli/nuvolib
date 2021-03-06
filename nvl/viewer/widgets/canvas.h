/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_CANVAS_H
#define NVL_VIEWER_CANVAS_H

#include <nvl/nuvolib.h>

#ifdef NVL_QT

#include <nvl/utilities/color.h>

#include <nvl/math/point.h>
#include <nvl/math/quaternion.h>
#include <nvl/math/alignedbox.h>

#include <nvl/viewer/interfaces/drawable.h>
#include <nvl/viewer/interfaces/animable.h>
#include <nvl/viewer/interfaces/pickable.h>
#include <nvl/viewer/interfaces/frameable.h>

#include <unordered_set>

namespace nvl {

class Canvas
{

public:    

    /* Picking */

    enum PickingIdentifier {
        PICKING_NONE,
        PICKING_DRAWABLE,
        PICKING_MESH_VERTEX, PICKING_MESH_POLYLINE, PICKING_MESH_FACE, PICKING_MESH_EDGE,
        PICKING_MESH_VERTEX_NORMAL,
        PICKING_MESH_FACE_EDGE, PICKING_MESH_FACE_NORMAL,
        PICKING_SKELETON_JOINT, PICKING_SKELETON_BONE
    };
    struct PickingData {
        PickingData();

        PickingIdentifier identifier;
        std::vector<Index> values;

        Index value(const Index& pos) const;
        void setValue(const Index& pos, const Index& value);
        void addValue(const Index& value);

        bool isEmpty() const;
    };


    /* Constructors/destructors */

    Canvas();
    virtual ~Canvas() = default;


    /* Methods */

    Size drawableNumber() const;

    const std::vector<Drawable*>& drawables() const;

    Drawable* drawable(const Index& id) const;
    virtual Index addDrawable(Drawable* drawable, const std::string& name = "", bool listable = true);
    virtual bool removeDrawable(const Index& id);
    virtual bool removeDrawable(Drawable* drawable);

    const std::string& drawableName(const Index& id) const;
    virtual void setDrawableName(const Index& id, const std::string& name);

    Animable* animable(const Index& id);
    Pickable* pickable(const Index& id);
    Frameable* frameable(const Index& id);

    bool isAnimable(const Index& id) const;
    bool isPickable(const Index& id) const;
    bool isFrameable(const Index& id) const;

    bool isListable(const Index& id) const;
    void setListable(const Index& id, const bool listable);

    const Affine3d& movableFrame() const;
    virtual void setMovableFrame(const Affine3d& frame);


    /* Abstract methods */

    virtual void updateGL() = 0;

    virtual void startAnimations();
    virtual void pauseAnimations();
    virtual void stopAnimations();
    virtual bool animationsRunning() const;
    virtual bool animationsPaused() const;

    virtual void setTargetFPS(double fps) = 0;
    virtual double targetFPS() = 0;

    virtual void setFPSDisplayed(bool display = true) = 0;
    virtual bool isFPSDisplayed() = 0;
    virtual void setAxisDisplayed(bool display = true) = 0;
    virtual bool isAxisDisplayed() = 0;

    virtual void setBackgroundColor(const Color& backgroundColor) = 0;
    virtual const Color& backgroundColor() const = 0;

    virtual void fitScene(bool visibleObjects = true) = 0;
    virtual void fitSceneSphere(const Point3d& center, const double radius) = 0;
    virtual void fitSceneBoundingBox(const AlignedBox3d& bbox) = 0;
    virtual void fitSceneBoundingBox(const Point3d& min, const Point3d& max) = 0;

    virtual unsigned int screenWidth() = 0;
    virtual unsigned int screenHeight() = 0;

    virtual Point3d cameraSceneCenter() const = 0;
    virtual void setCameraSceneCenter(const Point3d& cameraSceneCenter) = 0;
    virtual double cameraSceneRadius() const = 0;
    virtual void setCameraSceneRadius(const double radius) = 0;
    virtual Point3d cameraPosition() const = 0;
    virtual void setCameraPosition(const Point3d& cameraPosition) = 0;
    virtual Vector3d cameraViewDirection() const = 0;
    virtual void setCameraViewDirection(const Vector3d& viewDirection) const = 0;
    virtual Vector3d cameraUpVector() const = 0;
    virtual void setCameraUpVector(const Vector3d& upVector) const = 0;
    virtual Vector3d cameraRightVector() const = 0;
    virtual Quaterniond cameraOrientation() const = 0;
    virtual void setCameraOrientation(const Quaterniond& orientation) const = 0;
    virtual void setCameraLookAt(const Point3d& point) = 0;
    virtual Matrix44d cameraProjectionMatrix() const = 0;
    virtual Matrix44d cameraModelViewMatrix() const = 0;
    virtual Matrix44d cameraModelViewProjectionMatrix() const = 0;

    const std::vector<PickingData>& pickingDataPool() const;
    std::vector<PickingData>& pickingDataPool();
    const PickingData& pickingData(const int name) const;
    PickingData& pickingData(const int name);

protected:

    std::vector<Drawable*> vDrawables;
    std::vector<Pickable*> vPickables;
    std::vector<Animable*> vAnimables;
    std::vector<Frameable*> vFrameables;
    std::vector<std::string> vNames;
    std::vector<bool> vListable;

    Affine3d vMovableFrame;

    std::vector<PickingData> vPickingDataPool;

    bool vAnimationsRunning;
    bool vAnimationsPaused;

};

}

#endif

#include "canvas.cpp"

#endif // NVL_VIEWER_CANVAS_H
