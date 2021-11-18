#ifndef NVL_MODELS_MODEL_IO_RIG_H
#define NVL_MODELS_MODEL_IO_RIG_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/model_io_data.h>

#include <string>

namespace nvl {

bool modelLoadDataFromRig(
        const std::string& filename,
        IOModelData& data,
        IOModelError& error,
        IOModelMode& mode);
bool modelSaveDataToRig(
        const std::string& filename,
        const IOModelData& data,
        IOModelError& error,
        const IOModelMode& mode);

}

#include "model_io_rig.cpp"

#endif // NVL_MODELS_MODEL_IO_RIG_H
