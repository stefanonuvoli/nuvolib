#ifndef NVL_MODELS_SKINNING_WEIGHTS_IO_SKW_H
#define NVL_MODELS_SKINNING_WEIGHTS_IO_SKW_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/skinning_weights_io_data.h>

namespace nvl {

template<class T>
bool skinningWeightsLoadDataFromSkw(
        const std::string& filename,
        IOSkinningWeightsData<T>& data,
        IOSkinningWeightsError& error,
        IOSkinningWeightsMode& mode);
template<class T>
bool skinningWeightsSaveDataToSkw(
        const std::string& filename,
        const IOSkinningWeightsData<T>& data,
        IOSkinningWeightsError& error,
        const IOSkinningWeightsMode& mode);

}

#include "skinning_weights_io_skw.cpp"

#endif // NVL_MODELS_SKINNING_WEIGHTS_IO_SKW_H
