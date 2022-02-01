/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "model_transformations.h"

#include <nvl/models/algorithms/mesh_transformations.h>
#include <nvl/models/algorithms/skeleton_transformations.h>
#include <nvl/models/algorithms/animation_transformations.h>
#include <nvl/models/algorithms/model_clean.h>

#include <nvl/math/rotation.h>

namespace nvl {

template<class Model, class T>
void modelApplyTransformation(Model& model, const T& transformation, const bool removeNonStandardTransformations)
{
    //Apply to mesh
    meshApplyTransformation(model.mesh, transformation);

    //Apply to skeleton
    skeletonApplyTransformation(model.skeleton, transformation);

    //Apply to each animation
    for (Index aId = 0; aId < model.animationNumber(); ++aId) {
        animationApplyTransformation(model.animation(aId), transformation);
    }

    if (removeNonStandardTransformations) {
        //Remove scaling and rotation in bind pose of the model, and animation scaling
        modelRemoveNonStandardTransformations(model);
    }
}

}
