#ifndef NVL_MODELS_SKINNING_WEIGHTS_IO_H
#define NVL_MODELS_SKINNING_WEIGHTS_IO_H

#include <nvl/models/skinning_weights_io_data.h>

#include <nvl/models/skinning_weights.h>

namespace nvl {

template<class S>
bool skinningWeightsLoadFromFile(
        const std::string& filename,
        S& skinningWeights,
        IOSkinningWeightsError& error = internal::dummyIOSkinningWeightsError,
        IOSkinningWeightsMode& mode = internal::dummyIOSkinningWeightsMode);
template<class S>
bool skinningWeightsSaveToFile(
        const std::string& filename,
        const S& skinningWeights,
        IOSkinningWeightsError& error,
        const IOSkinningWeightsMode& mode);


/* Raw data IO */

template<class T, class J>
void skinningWeightsLoadData(
        SkinningWeights<T>& skinningWeights,
        const std::vector<std::vector<J>>& weights);
template<class T, class J>
void skinningWeightsLoadData(
        const SkinningWeights<T>& skinningWeights,
        std::vector<std::vector<J>>& weights);


}

#include "skinning_weights_io.cpp"

#endif // NVL_MODELS_SKINNING_WEIGHTS_IO_H
