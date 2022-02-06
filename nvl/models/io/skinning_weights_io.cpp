/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "skinning_weights_io.h"

#include <nvl/models/io/skinning_weights_io_skw.h>

#include <nvl/utilities/string_utils.h>

namespace nvl {

/**
 * @brief Load skinning weights from file
 * @param filename Filename
 * @param skinningWeights Skinning weights
 * @param error Error
 * @param mode Mode
 * @return True if loading has been successful
 */
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

/**
 * @brief Save skinning weights from file
 * @param filename Filename
 * @param skinningWeights Skinning weights
 * @param error Error
 * @param mode Mode
 * @return True if saving has been successful
 */
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

/**
 * @brief Load skinning weights data
 * @param skinningWeights Skinning weights
 * @param skinningWeightsData Skinning weights data
 * @param mode Mode
 */
template<class W>
void skinningWeightsLoadData(
        SkinningWeights<W>& skinningWeights,
        const IOSkinningWeightsData<W>& skinningWeightsData,
        const IOSkinningWeightsMode& mode)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mode);

    for (const std::tuple<Index, Index, W>& tuple : skinningWeightsData.weights) {
        skinningWeights.setWeight(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
    }

    skinningWeights.updateNonZeros();
}

/**
 * @brief Save skinning weights data
 * @param skinningWeights Skinning weights
 * @param skinningWeightsData Skinning weights data
 * @param mode Mode
 */
template<class W>
void skinningWeightsSaveData(
        const SkinningWeights<W>& skinningWeights,
        IOSkinningWeightsData<W>& skinningWeightsData,
        const IOSkinningWeightsMode& mode)
{    
    NVL_SUPPRESS_UNUSEDVARIABLE(mode);

    const std::vector<std::vector<Index>>& nonZeros = skinningWeights.nonZeroWeights();

    for (Index v = 0; v < nonZeros.size(); v++) {
        for (const Index& j : nonZeros[v]) {
            skinningWeightsData.weights.push_back(
                std::make_tuple(v, j, skinningWeights.weight(v, j)));
        }
    }
}

}
