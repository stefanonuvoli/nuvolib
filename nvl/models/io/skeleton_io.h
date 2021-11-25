#ifndef NVL_MODELS_SKELETON_IO_H
#define NVL_MODELS_SKELETON_IO_H

#include <nvl/models/io/skeleton_io_data.h>

#include <nvl/models/structures/skeleton.h>

namespace nvl {

template<class S>
bool skeletonLoadFromFile(
        const std::string& filename,
        S& skeleton,
        IOSkeletonError& error = internal::dummyIOSkeletonError,
        const IOSkeletonMode& mode = internal::dummyIOSkeletonMode);

template<class S>
bool skeletonSaveToFile(
        const std::string& filename,
        S& skeleton,
        IOSkeletonError& error = internal::dummyIOSkeletonError,
        const IOSkeletonMode& mode = internal::dummyIOSkeletonMode);


/* Raw data IO */

template<class S, class SD>
void skeletonLoadData(
        S& skeleton,
        const SD& skeletonData,
        const IOSkeletonMode& mode);

template<class Skeleton, class SD>
void skeletonSaveData(
        const Skeleton& skeleton,
        SD& skeletonData,
        const IOSkeletonMode& mode);

}

#include "skeleton_io.cpp"

#endif // NVL_MODELS_SKELETON_IO_H
