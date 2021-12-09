#include "model_transformations.h"

#include <nvl/models/algorithms/model_normalization.h>
#include <nvl/models/algorithms/mesh_transformations.h>
#include <nvl/models/algorithms/skeleton_transformations.h>
#include <nvl/models/algorithms/animation_transformations.h>
#include <nvl/models/algorithms/model_poses.h>

#include <nvl/math/rotation.h>

namespace nvl {

template<class Model, class T>
void modelApplyTransformation(Model& model, const Affine3<T>& transformation)
{
    typedef typename Model::Mesh Mesh;
    typedef typename Model::Skeleton Skeleton;
    typedef typename Model::Animation Animation;

    Skeleton& skeleton = model.skeleton;
    Mesh& mesh = model.mesh;

    //Remove rotation in bind pose of the model
    modelRemoveRotationInBindPose(model);

    //Apply to mesh
    meshApplyTransformation(mesh, transformation);

    //Apply to skeleton
    skeletonApplyTransformation(skeleton, transformation);

    //Apply to each animation
    for (Index aId = 0; aId < model.animationNumber(); ++aId) {
        Animation& animation = model.animation(aId);

        animationApplyTransformation(skeleton, animation, transformation);
    }
}


template<class Model>
void modelRemoveRotationInBindPose(Model& model)
{
    return modelRemoveRotationInBindPose(model.skeleton, model.animations);
}

template<class S, class A>
void modelRemoveRotationInBindPose(S& skeleton, std::vector<A>& animations)
{
    typedef S Skeleton;
    typedef A Animation;
    typedef typename Animation::Frame Frame;
    typedef typename Animation::FrameId FrameId;
    typedef typename Animation::Transformation AnimationTransformation;
    typedef typename Skeleton::Scalar Scalar;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Joint Joint;
    typedef typename Skeleton::Transformation SkeletonTransformation;

    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        Joint& joint = skeleton.joint(jId);

        Rotation3d jointRot(joint.bindPose().rotation());

        if (jointRot.isApprox(Rotation3d::Identity()))
            continue;

        Vector3<Scalar> vec = joint.bindPose() * Vector3<Scalar>(0,0,0);
        Translation3<Scalar> tra(vec);
        SkeletonTransformation newBindPose = tra * SkeletonTransformation::Identity();

        //Apply to each animation
        for (Index aId = 0; aId < animations.size(); ++aId) {
            Animation& animation = animations[aId];

            for (FrameId fId = 0; fId < animation.keyframeNumber(); ++fId) {
                Frame& frame = animation.keyframe(fId);

                std::vector<AnimationTransformation>& transformations = frame.transformations();

                //Get rotation transformation informations
                Rotation3<Scalar> animationRot(transformations[jId].rotation());
                Scalar angle = animationRot.angle();
                Vector3<Scalar> axis = animationRot.axis();
                axis = jointRot * axis;

                //Get translation transformation informations
                Vector3<Scalar> animationTra(transformations[jId].translation());
                animationTra = jointRot * animationTra;

                //Set new transformation
                transformations[jId].fromPositionOrientationScale(
                    animationTra,
                    Rotation3<Scalar>(angle, axis),
                    Vector3<Scalar>(1.0, 1.0, 1.0));
            }
        }

        joint.setBindPose(newBindPose);
    }

}


}
