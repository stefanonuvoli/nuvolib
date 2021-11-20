#include "model_io_data.h"

namespace nvl {

NVL_INLINE IOModelMode::IOModelMode() :
    mesh(true), skeleton(true), skinningWeights(true), animations(true),
    meshMode(IOMeshMode()), skeletonMode(IOSkeletonMode())
{

}

}
