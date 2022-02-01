/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_SKELETON_IO_SKT_H
#define NVL_MODELS_SKELETON_IO_SKT_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/skeleton_io_data.h>

#include <string>

namespace nvl {

template<class T>
bool skeletonLoadDataFromSKT(
        const std::string& filename,
        IOSkeletonData<T>& data,
        IOSkeletonError& error);

template<class T>
bool skeletonSaveDataToSKT(
        const std::string& filename,
        const IOSkeletonData<T>& data,
        IOSkeletonError& error);

}

#include "skeleton_io_skt.cpp"

#endif // NVL_MODELS_SKELETON_IO_SKT_H
