/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
