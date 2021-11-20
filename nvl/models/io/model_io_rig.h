#ifndef NVL_MODELS_MODEL_IO_RIG_H
#define NVL_MODELS_MODEL_IO_RIG_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/model_io_data.h>

#include <string>

namespace nvl {

template<class Model>
bool modelLoadModelFromRig(
        const std::string& filename,
        Model& model,
        IOModelError& error,
        IOModelMode& mode);
template<class Model>
bool modelSaveModelToRig(
        const std::string& filename,
        const Model& model,
        IOModelError& error,
        const IOModelMode& mode);

}

#include "model_io_rig.cpp"

#endif // NVL_MODELS_MODEL_IO_RIG_H
