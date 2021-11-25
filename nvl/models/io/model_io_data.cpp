#include "model_io_data.h"

namespace nvl {

NVL_INLINE IOModelMode::IOModelMode() :
    mesh(true), skeleton(true), skinningWeights(true), animations(true),
    meshMode(IOMeshMode()), skeletonMode(IOSkeletonMode())
{

}

template<class M, class S, class W, class A>
IOModelData<M,S,W,A>::IOModelData()
{
    clear();
}

template<class M, class S, class W, class A>
void IOModelData<M,S,W,A>::clear()
{
    name.clear();
    mesh.clear();
    skeleton.clear();
    skinningWeights.clear();
    animations.clear();
}

}
