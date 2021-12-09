#include "skeleton_transformations.h"

#include <vector>

#include <nvl/math/transformations.h>

namespace nvl {

template<class Skeleton, class T>
void skeletonApplyTransformation(Skeleton& skeleton, const T& transformation, const bool onlyTranslation)
{
    typedef typename Skeleton::Scalar Scalar;
    typedef typename Skeleton::JointId JointId;
    typedef typename Skeleton::Joint Joint;
    typedef typename Skeleton::Transformation SkeletonTransformation;

    if (onlyTranslation) {
        #pragma omp parallel for
        for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
            Joint& joint = skeleton.joint(jId);

            Vector3<Scalar> vec = joint.bindPose() * Vector3<Scalar>(0,0,0);
            Translation3<Scalar> tra(transformation  * vec);

            SkeletonTransformation newBindPose = tra * SkeletonTransformation::Identity();

            joint.setBindPose(newBindPose);
        }
    }
    else {
        #pragma omp parallel for
        for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
            Joint& joint = skeleton.joint(jId);

            joint.setBindPose(transformation * joint.bindPose());
        }
    }
}

}
