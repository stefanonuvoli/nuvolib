#include "skinning_weights_io_data.h"

namespace nvl {

NVL_INLINE IOSkinningWeightsMode::IOSkinningWeightsMode()
{

}

template<class V>
IOSkinningWeightsData<V>::IOSkinningWeightsData()
{
    clear();
}

template<class V>
void IOSkinningWeightsData<V>::clear()
{
    weights.clear();
}

}
