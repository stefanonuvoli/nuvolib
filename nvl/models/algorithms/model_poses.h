#ifndef NVL_MODELS_MODEL_POSES_H
#define NVL_MODELS_MODEL_POSES_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

/* ----------------------- DEFORMATION FROM LOCAL ----------------------- */

template<class M>
void modelDeformationFromLocal(
        M& model);


/* ----------------------- LOCAL FROM GLOBAL ----------------------- */

template<class M>
void modelLocalFromGlobal(
        M& model);


/* ----------------------- GLOBAL FROM LOCAL ----------------------- */

template<class M>
void modelGlobalFromLocal(
        M& model);

}

#include "model_poses.cpp"

#endif // NVL_MODELS_MODEL_POSES_H
