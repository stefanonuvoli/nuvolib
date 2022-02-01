/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MODEL_IO_H
#define NVL_MODELS_MODEL_IO_H

#include <nvl/models/io/model_io_data.h>

namespace nvl {

template<class M>
bool modelLoadFromFile(
        const std::string& filename,
        M& model,
        IOModelError& error = internal::dummyIOModelError,
        const IOModelMode& mode = internal::dummyIOModelMode);
template<class M>
bool modelSaveToFile(
        const std::string& filename,
        const M& model,
        IOModelError& error = internal::dummyIOModelError,
        const IOModelMode& mode = internal::dummyIOModelMode);

/* Raw data IO */

template<class M, class MD>
void modelLoadData(
        M& model,
        const MD& modelData,
        const IOModelMode& mode);

template<class M, class MD>
void modelSaveData(
        const M& model,
        MD& modelData,
        const IOModelMode& mode);

}

#include "model_io.cpp"

#endif // NVL_MODELS_MODEL_IO_H
