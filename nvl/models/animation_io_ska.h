#ifndef NVL_MODELS_ANIMATION_IO_SKA_H
#define NVL_MODELS_ANIMATION_IO_SKA_H

#include <nvl/nuvolib.h>

#include <nvl/models/animation_io_data.h>

#include <string>

namespace nvl {

template<class T>
bool animationLoadDataFromSka(
        const std::string& filename,
        IOAnimationData<T>& data,
        IOAnimationError& error,
        IOAnimationMode& mode);
template<class T>
bool animationSaveDataToSka(
        const std::string& filename,
        const IOAnimationData<T>& data,
        IOAnimationError& error,
        const IOAnimationMode& mode);

}

#include "animation_io_ska.cpp"

#endif // NVL_MODELS_ANIMATION_IO_SKA_H
