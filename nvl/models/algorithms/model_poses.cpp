#include "model_poses.h"

#include <nvl/models/algorithms/animation_poses.h>

namespace nvl {

/* ----------------------- DEFORMATION FROM LOCAL ----------------------- */

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
