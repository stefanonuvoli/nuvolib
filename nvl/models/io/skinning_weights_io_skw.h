#ifndef NVL_MODELS_SKINNING_WEIGHTS_IO_SKW_H
#define NVL_MODELS_SKINNING_WEIGHTS_IO_SKW_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/skinning_weights_io_data.h>

namespace nvl {

template<class T>
bool skinningWeightsLoadDataFromSKW(
        const std::string& filename,
        IOSkinningWeightsData<T>& data,
        IOSkinningWeightsError& error);
template<class T>
bool skinningWeightsSaveDataToSKW(
        const std::string& filename,
        const IOSkinningWeightsData<T>& data,
        IOSkinningWeightsError& error);

}

#include "skinning_weights_io_skw.cpp"

#endif // NVL_MODELS_SKINNING_WEIGHTS_IO_SKW_H
