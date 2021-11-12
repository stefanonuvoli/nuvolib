#include "model_deformation.h"

namespace nvl {

template<class M, class T>
void modelDeformLinearBlendingSkinning(
        M& model,
        const std::vector<T>& transformations)
{
    typedef typename M::SkinningWeights SkinningWeights;
    typedef typename M::Skeleton::Transformation Transformation;
    typedef typename M::Skeleton::Scalar Scalar;

    const SkinningWeights& skinningWeights = model.skinningWeights;

    #pragma omp parallel for
    for (Index jId = 0; jId < model.skeleton.jointNumber(); ++jId) {
        const T& t = transformations[jId];

        Point3d p = model.skeleton.joint(jId).restPose() * Point3<Scalar>(0,0,0);
        p = t * p;

        nvl::Translation3d tra(p);

        const Transformation r = tra * Transformation::Identity();

        model.skeleton.joint(jId).setRestPose(r);
    }

    #pragma omp parallel for
    for (Index vId = 0; vId < model.mesh.nextVertexId(); ++vId) {
        if (!model.mesh.isVertexDeleted(vId)) {
            T t = nvl::animationLinearBlendingSkinningVertex(skinningWeights, transformations, vId);

            const Point3d& p = model.mesh.vertex(vId).point();
            const Vector3d& n = model.mesh.vertexNormal(vId);

            model.mesh.vertex(vId).setPoint(t * p);
            model.mesh.vertex(vId).setNormal(t.rotation() * n);
        }
    }
}

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<DualQuaternion<T>>& transformations)
{
    typedef typename M::SkinningWeights SkinningWeights;
    typedef typename M::Skeleton::Transformation Transformation;
    typedef typename M::Skeleton::Scalar Scalar;

    const SkinningWeights& skinningWeights = model.skinningWeights;

    #pragma omp parallel for
    for (Index jId = 0; jId < model.skeleton.jointNumber(); ++jId) {
        const DualQuaterniond& dq = transformations[jId];

        Point3d p = model.skeleton.joint(jId).restPose() * Point3<Scalar>(0,0,0);
        p = dq * p;

        nvl::Translation3d tra(p);

        const Transformation r = tra * Transformation::Identity();

        model.skeleton.joint(jId).setRestPose(r);
    }

    #pragma omp parallel for
    for (Index vId = 0; vId < model.mesh.nextVertexId(); ++vId) {
        if (!model.mesh.isVertexDeleted(vId)) {
            DualQuaterniond dq = nvl::animationDualQuaternionSkinningVertex(skinningWeights, transformations, vId);

            const Point3d& p = model.mesh.vertex(vId).point();
            const Vector3d& n = model.mesh.vertexNormal(vId);

            model.mesh.vertex(vId).setPoint(dq * p);
            model.mesh.vertex(vId).setNormal(dq.rotation() * n);
        }
    }
}

template<class M, class T>
void modelDeformDualQuaternionSkinning(
        M& model,
        const std::vector<T>& transformations)
{
    typedef typename T::Scalar Scalar;

    std::vector<DualQuaternion<Scalar>> dualQuaternionTransformations(transformations.size());

    #pragma omp parallel for
    for (Index i = 0; i < transformations.size(); ++i) {
        dualQuaternionTransformations[i] = DualQuaternion<Scalar>(Quaternion<Scalar>(transformations[i].rotation()), transformations[i].translation());
    }

    modelDeformDualQuaternionSkinning(model, dualQuaternionTransformations);
}

}
