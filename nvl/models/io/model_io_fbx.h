#ifndef NVL_MODELS_MODEL_IO_FBX_H
#define NVL_MODELS_MODEL_IO_FBX_H

#ifdef NVL_FBXSDK_LOADED

#include <nvl/nuvolib.h>

#include <nvl/models/io/model_io_data.h>

#include <string>

namespace nvl {

template<class M, class S, class W, class A>
bool modelLoadDataFromFBX(
        const std::string& filename,
        IOModelData<M,S,W,A>& modelData,
        IOModelError& error,
        const IOModelMode& mode);

}

#include "model_io_fbx.cpp"

#endif

#endif // NVL_MODELS_MODEL_IO_FBX_H
