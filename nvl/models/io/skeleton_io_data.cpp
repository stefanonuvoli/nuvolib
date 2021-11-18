#include "skeleton_io_data.h"

namespace nvl {

NVL_INLINE IOSkeletonMode::IOSkeletonMode()
{

}

template<class V>
IOSkeletonData<V>::IOSkeletonData()
{
    clear();
}

template<class V>
void IOSkeletonData<V>::clear()
{
    joints.clear();
    parents.clear();
    names.clear();
}

}
