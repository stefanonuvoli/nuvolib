#include "skinning_weights_io.h"

#include <nvl/models/io/skinning_weights_io_skw.h>

namespace nvl {

template<class S>
bool skinningWeightsLoadFromFile(
        const std::string& filename,
        S& skinningWeights,
        IOSkinningWeightsError& error,
        IOSkinningWeightsMode& mode)
{
    IOSkinningWeightsData<typename S::Scalar> data;

    std::string ext = filenameExtension(filename);

    bool success;
    if (ext == "skw") {
        success = skinningWeightsLoadDataFromSkw(filename, data, error, mode);
    }
    else {
        error = IO_SKINNINGWEIGHTS_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        skinningWeightsLoadData(skinningWeights, data.weights);
    }

    return success;
}

template<class S>
bool skinningWeightsSaveToFile(
        const std::string& filename,
        const S& skinningWeights,
        IOSkinningWeightsError& error,
        const IOSkinningWeightsMode& mode)
{
    std::string ext = filenameExtension(filename);

    bool success;
    if (ext == "skw") {
        IOSkinningWeightsData<typename S::Scalar> data;
        skinningWeightsSaveData(skinningWeights, data.weights);

        success = skinningWeightsSaveDataToSkw(filename, data, error, mode);
    }
    else {
        error = IO_SKINNINGWEIGHTS_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}


template<class T, class J>
void skinningWeightsLoadData(
        SkinningWeights<T>& skinningWeights,
        const std::vector<std::tuple<Index, Index, J>>& weights)
{
    for (const std::tuple<Index, Index, J>& tuple : weights) {
        skinningWeights.setWeight(std::get<1>(tuple), std::get<0>(tuple), std::get<2>(tuple));
    }
    skinningWeights.updateNonZeros();
}

template<class T, class J>
void skinningWeightsSaveData(
        const SkinningWeights<T>& skinningWeights,
        std::vector<std::tuple<Index, Index, J>>& weights)
{
    const std::vector<std::vector<Index>>& nonZeros = skinningWeights.nonZeroWeights();

    for (Index v = 0; v < nonZeros.size(); v++) {
        for (const Index& j : nonZeros[v]) {
            weights.push_back(std::make_tuple(j, v, skinningWeights.weight(v, j)));
        }
    }

}

}
