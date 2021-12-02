#include "skinning_weights_io.h"

#include <nvl/models/io/skinning_weights_io_skw.h>

#include <nvl/utilities/string_utils.h>

namespace nvl {

template<class W>
bool skinningWeightsLoadFromFile(
        const std::string& filename,
        W& skinningWeights,
        IOSkinningWeightsError& error,
        const IOSkinningWeightsMode& mode)
{
    IOSkinningWeightsData<typename W::Scalar> skinningWeightsData;

    std::string ext = stringToLower(filenameExtension(filename));

    bool success;
    if (ext == "skw") {
        success = skinningWeightsLoadDataFromSKW(filename, skinningWeightsData, error);
    }
    else {
        error = IO_SKINNINGWEIGHTS_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        skinningWeightsLoadData(skinningWeights, skinningWeightsData, mode);
    }

    return success;
}

template<class W>
bool skinningWeightsSaveToFile(
        const std::string& filename,
        const W& skinningWeights,
        IOSkinningWeightsError& error,
        const IOSkinningWeightsMode& mode)
{
    std::string ext = stringToLower(filenameExtension(filename));

    bool success;
    if (ext == "skw") {
        IOSkinningWeightsData<typename W::Scalar> skinningWeightsData;

        skinningWeightsSaveData(skinningWeights, skinningWeightsData, mode);

        success = skinningWeightsSaveDataToSKW(filename, skinningWeightsData, error);
    }
    else {
        error = IO_SKINNINGWEIGHTS_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}


template<class W>
void skinningWeightsLoadData(
        SkinningWeights<W>& skinningWeights,
        const IOSkinningWeightsData<W>& skinningWeightsData,
        const IOSkinningWeightsMode& mode)
{
    for (const std::tuple<Index, Index, W>& tuple : skinningWeightsData.weights) {
        skinningWeights.setWeight(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
    }

    skinningWeights.updateNonZeros();
}

template<class W>
void skinningWeightsSaveData(
        const SkinningWeights<W>& skinningWeights,
        IOSkinningWeightsData<W>& skinningWeightsData,
        const IOSkinningWeightsMode& mode)
{
    const std::vector<std::vector<Index>>& nonZeros = skinningWeights.nonZeroWeights();

    for (Index v = 0; v < nonZeros.size(); v++) {
        for (const Index& j : nonZeros[v]) {
            skinningWeightsData.weights.push_back(
                std::make_tuple(v, j, skinningWeights.weight(v, j)));
        }
    }
}

}
