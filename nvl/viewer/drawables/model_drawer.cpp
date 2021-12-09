#include "model_drawer.h"

#include <nvl/models/algorithms/model_pose_propagation.h>
#include <nvl/models/algorithms/animation_blend.h>
#include <nvl/models/algorithms/animation_skinning.h>

#include <chrono>

namespace nvl {

template<class M>
ModelDrawer<M>::ModelDrawer() : ModelDrawer(nullptr)
{

}

template<class M>
ModelDrawer<M>::ModelDrawer(M* model, const bool visible, const bool pickable, const bool animable) :
    Drawable(visible),
    Pickable(pickable),
    Animable(animable),
    vModel(model),
    vMeshDrawer(&model->mesh),
    vSkeletonDrawer(&model->skeleton),
    vAnimationLoaded(NULL_ID),
    vAnimationRunning(false),
    vAnimationPaused(false)
{
    vMeshDrawer.setFaceShadingMode(FaceMeshDrawer<typename M::Mesh>::FACE_SHADING_SMOOTH);
    vMeshDrawer.setFaceColorMode(FaceMeshDrawer<typename M::Mesh>::FACE_COLOR_PER_VERTEX);
    vSkeletonDrawer.setVisible(false);
}

template<class M>
void ModelDrawer<M>::draw() const
{
    if (this->vModel == nullptr)
        return;

    vMeshDrawer.draw();

    glDepthFunc(GL_ALWAYS);
    vSkeletonDrawer.draw();
    glDepthFunc(GL_LESS);
}


template<class M>
void ModelDrawer<M>::drawWithNames(Canvas* canvas, const Index drawableId) const
{
    if (this->vModel == nullptr || !this->isPickable())
        return;

    vMeshDrawer.drawWithNames(canvas, drawableId);

    glDepthFunc(GL_ALWAYS);
    vSkeletonDrawer.drawWithNames(canvas, drawableId);
    glDepthFunc(GL_LESS);
}

template<class M>
void ModelDrawer<M>::startAnimation()
{
    if (this->vAnimable && isAnimationLoaded()) {
        if (!vAnimationPaused) {
            resetAnimation();
            vAnimationStartTime = std::chrono::system_clock::now();
            vAnimationRunning = true;
        }
        else {
            vAnimationStartTime = std::chrono::system_clock::now() - (vAnimationPauseTime - vAnimationStartTime);
            vAnimationPaused = false;
        }
    }
}

template<class M>
void ModelDrawer<M>::pauseAnimation()
{
    if (this->vAnimable && isAnimationLoaded() && vAnimationRunning) {
        vAnimationPauseTime = std::chrono::system_clock::now();
        vAnimationPaused = true;
    }
}

template<class M>
void ModelDrawer<M>::stopAnimation()
{
    if (this->vAnimable && isAnimationLoaded()) {
        resetAnimation();
    }

    vAnimationRunning = false;
    vAnimationPaused = false;
}

template<class M>
bool ModelDrawer<M>::animate()
{
    if (this->vAnimable && this->vAnimationRunning) {
        if (!this->vAnimationPaused) {
            const Index lastFrameId = vAnimationFrames.size() - 1;

            if (vAnimationCurrentFrameId == lastFrameId) {
                if (this->vAnimationLoop) {
                    vAnimationStartTime = std::chrono::system_clock::now();
                    resetAnimation();
                    return true;
                }
                else {
                    vAnimationRunning = false;
                    vAnimationPaused = false;
                    resetAnimation();
                    return false;
                }
            }

            const double microsecondConversion = this->vAnimationSpeed / 1000000.0;
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - vAnimationStartTime).count() * microsecondConversion;

            while (vAnimationCurrentFrameId < lastFrameId && vAnimationFrames[vAnimationCurrentFrameId + 1].time() <= elapsed) {
                ++vAnimationCurrentFrameId;
            }
            processKeyframePose(vAnimationCurrentFrameId);
        }

        return true;
    }
    else {
        return false;
    }
}

template<class M>
bool ModelDrawer<M>::animationRunning() const
{
    return vAnimationRunning;
}

template<class M>
Point3d ModelDrawer<M>::sceneCenter() const
{
    return this->vFrame * vMeshDrawer.sceneCenter();
}

template<class M>
double ModelDrawer<M>::sceneRadius() const
{
    Point3d min = this->vFrame * vMeshDrawer.boundingBox().min();
    Point3d max = this->vFrame * vMeshDrawer.boundingBox().max();

    Vector3d vec = max - min;

    return vec.norm() / 2;
}

template<class M>
void ModelDrawer<M>::resetRenderingData()
{
    vMeshDrawer.resetRenderingData();
    vSkeletonDrawer.resetRenderingData();
}

template<class M>
void ModelDrawer<M>::update()
{
    if (vModel == nullptr)
        return;

    vMeshDrawer.update();
    vSkeletonDrawer.update();

    if (isAnimationLoaded()) {
        loadAnimation(vAnimationLoaded);
    }
}

template<class M>
M* ModelDrawer<M>::model() const
{
    return vModel;
}

template<class M>
void ModelDrawer<M>::setModel(M* model)
{
    vModel = model;
}

template<class M>
FaceMeshDrawer<typename M::Mesh>& ModelDrawer<M>::meshDrawer()
{
    return vMeshDrawer;
}

template<class M>
SkeletonDrawer<typename M::Skeleton>& ModelDrawer<M>::skeletonDrawer()
{
    return vSkeletonDrawer;
}

template<class M>
void ModelDrawer<M>::loadAnimation(const Index& id)
{
    typedef typename Model::Animation Animation;
    typedef typename Animation::Frame Frame;
    typedef typename Frame::Transformation Transformation;
    typedef typename Transformation::Scalar Scalar;

    vAnimationLoaded = id;
    const Animation3d& animation = vModel->animation(id);

    //Blend frames
    vAnimationFrames = animation.keyframes();
    if (this->animationBlend()) {
        double fps = this->animationTargetFPS() / this->animationSpeed();
        animationBlendFrameTransformations(vAnimationFrames, fps, this->animationSpeed(), this->animationKeepKeyframes());
    }

    //Compute final transformations
    animationFrameDeformationFromLocal(*(vSkeletonDrawer.skeleton()), vAnimationFrames);

    if (this->vAnimationSkinningMode == SkinningMode::SKINNING_DUAL_QUATERNIONS) {
        dualQuaternionTransformations.resize(vAnimationFrames.size());
        #pragma omp parallel for
        for (Index i = 0; i < vAnimationFrames.size(); ++i) {
            const std::vector<Transformation>& transformations = vAnimationFrames[i].transformations();

            dualQuaternionTransformations[i].resize(transformations.size());

            #pragma omp parallel for
            for (Index j = 0; j < transformations.size(); ++j) {
                dualQuaternionTransformations[i][j] = DualQuaternion<Scalar>(Quaternion<Scalar>(transformations[j].rotation()), transformations[j].translation());
            }
        }
    }

    resetAnimation();
    vAnimationStartTime = std::chrono::system_clock::now();
}

template<class M>
void ModelDrawer<M>::resetAnimation()
{
    this->vMeshDrawer.resetRenderingVertices();
    this->vMeshDrawer.resetRenderingVertexNormals();
    this->vSkeletonDrawer.resetRenderingJoints();

    vAnimationCurrentFrameId = 0;
    processKeyframePose(0);
}

template<class M>
void ModelDrawer<M>::unloadAnimation()
{
    vAnimationLoaded = NULL_ID;
    vAnimationFrames.clear();
    dualQuaternionTransformations.clear();
    vAnimationCurrentFrameId = NULL_ID;
    vAnimationRunning = false;

    this->vMeshDrawer.resetRenderingVertices();
    this->vMeshDrawer.resetRenderingVertexNormals();
    this->vSkeletonDrawer.resetRenderingJoints();
}

template<class M>
bool ModelDrawer<M>::isAnimationLoaded() const
{
    return vAnimationLoaded != NULL_ID;
}

template<class M>
Index ModelDrawer<M>::loadedAnimation() const
{
    return vAnimationLoaded;
}

template<class M>
void ModelDrawer<M>::setCurrentFrameId(const Index& kId)
{
    if (isAnimationLoaded()) {
        vAnimationCurrentFrameId = kId;
        processKeyframePose(vAnimationCurrentFrameId);
    }
}

template<class M>
const Index& ModelDrawer<M>::currentFrameId()
{
    return vAnimationCurrentFrameId;
}

template<class M>
const typename Animation3d::Frame& ModelDrawer<M>::currentFrame()
{
    return vAnimationFrames[vAnimationCurrentFrameId];
}

template<class M>
Size ModelDrawer<M>::keyframeNumber()
{
    return vAnimationFrames.size();
}

template<class M>
template<class T>
void ModelDrawer<M>::renderLinearBlendingSkinning(const std::vector<T>& transformations)
{
    typedef typename Model::SkinningWeights SkinningWeights;
    typedef typename Model::Skeleton::JointId JointId;
    typedef typename Model::Mesh::VertexId VertexId;

    const SkinningWeights& skinningWeights = this->vModel->skinningWeights;

    #pragma omp parallel for
    for (JointId jId = 0; jId < this->vModel->skeleton.jointNumber(); ++jId) {
        if (!this->vModel->skeleton.jointIsHidden(jId)) {
            const T& t = transformations[jId];

            const Point3d p = this->vModel->skeleton.jointBindPose(jId) * this->vModel->skeleton.originPoint();

            vSkeletonDrawer.setRenderingJoint(jId, t * p);
        }
    }

    #pragma omp parallel for
    for (VertexId vId = 0; vId < this->vModel->mesh.nextVertexId(); ++vId) {
        if (!this->vModel->mesh.isVertexDeleted(vId)) {
            T t = animationLinearBlendingSkinningVertex(skinningWeights, transformations, vId);

            const Point3d& p = this->vModel->mesh.vertexPoint(vId);
            const Vector3d& n = this->vModel->mesh.vertexNormal(vId);

            vMeshDrawer.setRenderingVertex(vId, t * p);
            vMeshDrawer.setRenderingVertexNormal(vId, t.rotation() * n);
        }
    }
}

template<class M>
void ModelDrawer<M>::renderDualQuaternionSkinning(const std::vector<DualQuaterniond>& transformations)
{
    typedef typename Model::SkinningWeights SkinningWeights;
    typedef typename Model::Skeleton::JointId JointId;
    typedef typename Model::Mesh::VertexId VertexId;

    const SkinningWeights& skinningWeights = this->vModel->skinningWeights;

    #pragma omp parallel for
    for (JointId jId = 0; jId < this->vModel->skeleton.jointNumber(); ++jId) {
        if (!this->vModel->skeleton.jointIsHidden(jId)) {
            const DualQuaterniond& dq = transformations[jId];

            const Point3d p = this->vModel->skeleton.jointBindPose(jId) * this->vModel->skeleton.originPoint();

            vSkeletonDrawer.setRenderingJoint(jId, dq * p);
        }
    }

    #pragma omp parallel for
    for (VertexId vId = 0; vId < this->vModel->mesh.nextVertexId(); ++vId) {
        if (!this->vModel->mesh.isVertexDeleted(vId)) {
            DualQuaterniond dq = animationDualQuaternionSkinningVertex(skinningWeights, transformations, vId);

            const Point3d& p = this->vModel->mesh.vertexPoint(vId);
            const Vector3d& n = this->vModel->mesh.vertexNormal(vId);

            vMeshDrawer.setRenderingVertex(vId, dq * p);
            vMeshDrawer.setRenderingVertexNormal(vId, dq.rotation() * n);
        }
    }
}

template<class M>
void ModelDrawer<M>::processKeyframePose(const Index& frameId)
{
    typedef typename Model::Animation Animation;
    typedef typename Animation::Frame Frame;
    typedef typename Frame::Transformation Transformation;

    if (this->vAnimationSkinningMode == SkinningMode::SKINNING_LINEAR_BLENDING) {
        const Frame& frame = vAnimationFrames[frameId];

        const std::vector<Transformation>& transformations = frame.transformations();

        this->renderLinearBlendingSkinning(transformations);
    }
    else {
        assert(this->vAnimationSkinningMode == SkinningMode::SKINNING_DUAL_QUATERNIONS);

        const std::vector<DualQuaterniond>& transformations = dualQuaternionTransformations[frameId];

        this->renderDualQuaternionSkinning(transformations);
    }
}

}
