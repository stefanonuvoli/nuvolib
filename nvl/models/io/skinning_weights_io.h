/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_SKINNING_WEIGHTS_IO_H
#define NVL_MODELS_SKINNING_WEIGHTS_IO_H

#include <nvl/models/io/skinning_weights_io_data.h>

#include <nvl/models/structures/skinning_weights.h>

namespace nvl {

template<class W>
bool skinningWeightsLoadFromFile(
        const std::string& filename,
        W& skinningWeights,
        IOSkinningWeightsError& error = internal::dummyIOSkinningWeightsError,
        const IOSkinningWeightsMode& mode = internal::dummyIOSkinningWeightsMode);
template<class W>
bool skinningWeightsSaveToFile(
        const std::string& filename,
        const W& skinningWeights,
        IOSkinningWeightsError& error,
        const IOSkinningWeightsMode& mode);


/* Raw data IO */

template<class W>
void skinningWeightsLoadData(
        SkinningWeights<W>& skinningWeights,
        const IOSkinningWeightsData<W>& skinningWeightsData,
        const IOSkinningWeightsMode& mode);

template<class W>
void skinningWeightsSaveData(
        const SkinningWeights<W>& skinningWeights,
        IOSkinningWeightsData<W>& skinningWeightsData,
        const IOSkinningWeightsMode& mode);


}

#include "skinning_weights_io.cpp"

#endif // NVL_MODELS_SKINNING_WEIGHTS_IO_H
