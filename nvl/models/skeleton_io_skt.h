#ifndef NVL_MODELS_SKELETON_IO_SKT_H
#define NVL_MODELS_SKELETON_IO_SKT_H

#include <nvl/nuvolib.h>

#include <nvl/models/skeleton_io_data.h>

#include <string>

namespace nvl {

template<class V>
bool skeletonLoadDataFromSkt(
        const std::string& filename,
        IOSkeletonData<V>& data,
        IOSkeletonError& error,
        IOSkeletonMode& mode);

template<class V>
bool skeletonSaveDataToSkt(
        const std::string& filename,
        const IOSkeletonData<V>& data,
        IOSkeletonError& error,
        const IOSkeletonMode& mode);

}

#include "skeleton_io_skt.cpp"

#endif // NVL_MODELS_SKELETON_IO_SKT_H
