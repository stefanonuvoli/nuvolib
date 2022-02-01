/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_ANIMATION_IO_H
#define NVL_MODELS_ANIMATION_IO_H

#include <nvl/models/io/animation_io_data.h>

#include <nvl/models/structures/animation.h>

namespace nvl {

template<class A>
bool animationLoadFromFile(
        const std::string& filename,
        A& animation,
        IOAnimationError& error = internal::dummyIOAnimationError,
        const IOAnimationMode& mode = internal::dummyIOAnimationMode);
template<class A>
bool animationSaveToFile(
        const std::string& filename,
        const A& animation,
        IOAnimationError& error = internal::dummyIOAnimationError,
        const IOAnimationMode& mode = internal::dummyIOAnimationMode);

/* Raw data IO */

template<class A, class AD>
void animationLoadData(
        A& animation,
        const AD& animationData,
        const IOAnimationMode& mode);

template<class A, class AD>
void animationSaveData(
        const A& animation,
        AD& animationData,
        const IOAnimationMode& mode);


}

#include "animation_io.cpp"

#endif // NVL_MODELS_ANIMATION_IO_H
