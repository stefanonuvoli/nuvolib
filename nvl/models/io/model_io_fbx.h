#ifndef NVL_MODELS_MODEL_IO_FBX_H
#define NVL_MODELS_MODEL_IO_FBX_H

#ifdef NVL_FBXSDK_LOADED

#include <nvl/nuvolib.h>

#include <nvl/models/io/model_io_data.h>

#include <string>

namespace nvl {

template<class Model>
bool modelLoadModelsFromFBX(
        const std::string& filename,
        std::vector<Model>& models,
        IOModelError& error,
        IOModelMode& mode);

}

#include "model_io_fbx.cpp"

#endif

#endif // NVL_MODELS_MODEL_IO_FBX_H
