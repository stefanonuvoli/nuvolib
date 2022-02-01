/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "skinning_weights_io_data.h"

namespace nvl {

NVL_INLINE IOSkinningWeightsMode::IOSkinningWeightsMode()
{

}

template<class S>
IOSkinningWeightsData<S>::IOSkinningWeightsData()
{
    clear();
}

template<class S>
void IOSkinningWeightsData<S>::clear()
{
    weights.clear();
}

}
