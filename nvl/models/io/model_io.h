#ifndef NVL_MODELS_MODEL_IO_H
#define NVL_MODELS_MODEL_IO_H

#include <nvl/models/io/model_io_data.h>

namespace nvl {

template<class Model>
bool modelLoadFromFile(
        const std::string& filename,
        Model& model,
        IOModelError& error = internal::dummyIOModelError,
        IOModelMode& mode = internal::dummyIOModelMode);
template<class Model>
bool modelSaveToFile(
        const std::string& filename,
        const Model& model,
        IOModelError& error = internal::dummyIOModelError,
        const IOModelMode& mode = internal::dummyIOModelMode);

}

#include "model_io.cpp"

#endif // NVL_MODELS_MODEL_IO_H
