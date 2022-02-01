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
