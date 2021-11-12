#include "animation_io_data.h"

namespace nvl {

NVL_INLINE IOAnimationMode::IOAnimationMode()
{

}

template<class T>
void IOAnimationData<T>::clear()
{
    name.clear();
    times.clear();
    transformations.clear();
}

template<class T>
IOAnimationData<T>::IOAnimationData()
{
    clear();
}

}
