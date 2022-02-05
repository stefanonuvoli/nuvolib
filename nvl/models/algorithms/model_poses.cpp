/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "model_poses.h"

#include <nvl/models/algorithms/animation_poses.h>

namespace nvl {

/* ----------------------- DEFORMATION FROM LOCAL AND GLOBAL ----------------------- */

/**
 * @brief Calculate model deformation from global transformations
 * @param model Model
 */
template<class M>
void modelDeformationFromGlobal(
        M& model)
{
    typedef M Model;
    typedef typename Model::Animation Animation;

    for (Animation& a : model.animations) {
        animationDeformationFromGlobal(model->skeleton, a);
    }
}

/**
 * @brief Calculate model deformation from local transformations
 * @param model Model
 */
template<class M>
void modelDeformationFromLocal(
        M& model)
{
    typedef M Model;
    typedef typename Model::Animation Animation;

    for (Animation& a : model.animations) {
        animationDeformationFromLocal(model->skeleton, a);
    }
}


/* ----------------------- LOCAL FROM GLOBAL ----------------------- */

/**
 * @brief Calculate local transformations from global transformations
 * @param model Model
 */
template<class M>
void modelLocalFromGlobal(
        M& model)
{
    typedef M Model;
    typedef typename Model::Animation Animation;

    for (Animation& a : model.animations) {
        animationLocalFromGlobal(model, a);
    }
}


/* ----------------------- GLOBAL FROM LOCAL ----------------------- */

/**
 * @brief Calculate global transformations from local transformations
 * @param model Model
 */
template<class M>
void modelGlobalFromLocal(
        M& model)
{
    typedef M Model;
    typedef typename Model::Animation Animation;

    for (Animation& a : model.animations) {
        animationGlobalFromLocal(model, a);
    }
}

}
