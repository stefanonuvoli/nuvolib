#ifndef NVL_MODELS_SKINNING_WEIGHTS_IO_DATA_H
#define NVL_MODELS_SKINNING_WEIGHTS_IO_DATA_H

#include <nvl/nuvolib.h>

#include <vector>
#include <utility>

namespace nvl {

//Error messages
enum IOSkinningWeightsError { IO_SKINNINGWEIGHTS_SUCCESS, IO_SKINNINGWEIGHTS_FILE_ERROR, IO_SKINNINGWEIGHTS_FORMAT_NON_RECOGNISED, IO_SKINNINGWEIGHTS_EXTENSION_NON_SUPPORTED };

//IO mode
struct IOSkinningWeightsMode {
    IOSkinningWeightsMode();
};

//Skeleton weights
template<class S = double>
struct IOSkinningWeightsData {
    IOSkinningWeightsData();

    std::vector<std::tuple<Index, Index, S>> weights;

    void clear();
};

namespace internal {

//Dummy variables for default arguments
static IOSkinningWeightsError dummyIOSkinningWeightsError;
static IOSkinningWeightsMode dummyIOSkinningWeightsMode;

}

}

#include "skinning_weights_io_data.cpp"

#endif // NVL_MODELS_SKINNING_WEIGHTS_IO_DATA_H
