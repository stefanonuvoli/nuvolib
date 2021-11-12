#ifndef NVL_MODELS_ANIMATION_IO_H
#define NVL_MODELS_ANIMATION_IO_H

#include <nvl/models/animation_io_data.h>

#include <nvl/models/animation.h>

namespace nvl {

template<class A>
bool animationLoadFromFile(
        const std::string& filename,
        A& animation,
        IOAnimationError& error = internal::dummyIOAnimationError,
        IOAnimationMode& mode = internal::dummyIOAnimationMode);
template<class A>
bool animationSaveToFile(
        const std::string& filename,
        const A& animation,
        IOAnimationError& error = internal::dummyIOAnimationError,
        const IOAnimationMode& mode = internal::dummyIOAnimationMode);

/* Raw data IO */

template<class T>
void animationLoadData(
        Animation<T>& animation,
        const std::string& name,
        const std::vector<double>& times,
        const std::vector<std::vector<T>>& transformations);
template<class T>
void animationSaveData(
        const Animation<T>& animation,
        std::string& name,
        std::vector<double>& times,
        std::vector<std::vector<T>>& transformations);


}

#include "animation_io.cpp"

#endif // NVL_MODELS_ANIMATION_IO_H
