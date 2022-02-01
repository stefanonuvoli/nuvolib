/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_ANIMATION_IO_SKA_H
#define NVL_MODELS_ANIMATION_IO_SKA_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/animation_io_data.h>

#include <string>

namespace nvl {

template<class T>
bool animationLoadDataFromSKA(
        const std::string& filename,
        IOAnimationData<T>& data,
        IOAnimationError& error);
template<class T>
bool animationSaveDataToSKA(
        const std::string& filename,
        const IOAnimationData<T>& data,
        IOAnimationError& error);

}

#include "animation_io_ska.cpp"

#endif // NVL_MODELS_ANIMATION_IO_SKA_H
