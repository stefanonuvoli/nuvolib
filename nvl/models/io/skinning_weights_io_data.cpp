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
