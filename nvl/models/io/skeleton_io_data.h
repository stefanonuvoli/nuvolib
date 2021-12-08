#ifndef NVL_MODELS_SKELETON_IO_DATA_H
#define NVL_MODELS_SKELETON_IO_DATA_H

#include <nvl/nuvolib.h>

#include <string>
#include <vector>

#include <nvl/math/affine.h>

namespace nvl {

//Error messages
enum IOSkeletonError { IO_SKELETON_SUCCESS, IO_SKELETON_FILE_ERROR, IO_SKELETON_FORMAT_NON_RECOGNISED, IO_SKELETON_EXTENSION_NON_SUPPORTED };

//IO mode
struct IOSkeletonMode {
    IOSkeletonMode();
};

//Skeleton data
template<class T = Affine3d>
struct IOSkeletonData {
    IOSkeletonData();

    std::vector<T> joints;
    std::vector<bool> hidden;
    std::vector<int> parents;
    std::vector<std::string> names;

    void clear();
};

namespace internal {

//Dummy variables for default arguments
static IOSkeletonError dummyIOSkeletonError;
static IOSkeletonMode dummyIOSkeletonMode;

}

}

#include "skeleton_io_data.cpp"

#endif // NVL_MODELS_SKELETON_IO_DATA_H
