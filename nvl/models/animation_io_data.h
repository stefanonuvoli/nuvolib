#ifndef NVL_MODELS_ANIMATION_IO_DATA_H
#define NVL_MODELS_ANIMATION_IO_DATA_H

#include <nvl/nuvolib.h>

#include <nvl/math/transformations.h>

#include <vector>

namespace nvl {

//Error messages
enum IOAnimationError { IO_ANIMATION_SUCCESS, IO_ANIMATION_FILE_ERROR, IO_ANIMATION_FORMAT_NON_RECOGNISED, IO_ANIMATION_EXTENSION_NON_SUPPORTED };

//IO mode
struct IOAnimationMode {
    IOAnimationMode();
};

//Animations
template<class T = Affine3d>
struct IOAnimationData {
    IOAnimationData();

    std::string name;
    std::vector<double> times;
    std::vector<std::vector<T>> transformations;

    void clear();
};

namespace internal {

//Dummy variables for default arguments
static IOAnimationError dummyIOAnimationError;
static IOAnimationMode dummyIOAnimationMode;

}

}

#include "animation_io_data.cpp"

#endif // NVL_MODELS_ANIMATION_IO_DATA_H
