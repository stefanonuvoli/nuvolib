#ifndef NVL_MODELS_SKELETON_IO_SKT_H
#define NVL_MODELS_SKELETON_IO_SKT_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/skeleton_io_data.h>

#include <string>

namespace nvl {

template<class V>
bool skeletonLoadDataFromSKT(
        const std::string& filename,
        IOSkeletonData<V>& data,
        IOSkeletonError& error);

template<class V>
bool skeletonSaveDataToSKT(
        const std::string& filename,
        const IOSkeletonData<V>& data,
        IOSkeletonError& error);

}

#include "skeleton_io_skt.cpp"

#endif // NVL_MODELS_SKELETON_IO_SKT_H
