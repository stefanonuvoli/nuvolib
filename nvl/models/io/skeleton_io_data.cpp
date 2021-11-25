#include "skeleton_io_data.h"

namespace nvl {

NVL_INLINE IOSkeletonMode::IOSkeletonMode()
{

}

template<class T>
IOSkeletonData<T>::IOSkeletonData()
{
    clear();
}

template<class T>
void IOSkeletonData<T>::clear()
{
    joints.clear();
    parents.clear();
    names.clear();
}

}
