#ifndef NVL_MODELS_MODEL_IO_RIG_H
#define NVL_MODELS_MODEL_IO_RIG_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/model_io_data.h>

#include <string>

namespace nvl {

template<class M, class S, class W, class A>
bool modelLoadDataFromRIG(
        const std::string& filename,
        IOModelData<M,S,W,A>& modelData,
        IOModelError& error,
        const IOModelMode& mode);

template<class M, class S, class W, class A>
bool modelSaveDataToRIG(
        const std::string& filename,
        const IOModelData<M,S,W,A>& modelData,
        IOModelError& error,
        const IOModelMode& mode);

}

#include "model_io_rig.cpp"

#endif // NVL_MODELS_MODEL_IO_RIG_H
