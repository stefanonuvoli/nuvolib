/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
