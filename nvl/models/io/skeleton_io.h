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
        IOSkeletonMode& mode = internal::dummyIOSkeletonMode);

template<class S>
bool skeletonSaveToFile(
        const std::string& filename,
        S& skeleton,
        IOSkeletonError& error = internal::dummyIOSkeletonError,
        const IOSkeletonMode& mode = internal::dummyIOSkeletonMode);


/* Raw data IO */

template<class V, class T>
void skeletonLoadData(
        Skeleton<V>& skeleton,
        const std::vector<T>& joints,
        const std::vector<int>& parents,
        const std::vector<std::string>& names);

template<class V, class T>
void skeletonSaveData(
        const Skeleton<V>& skeleton,
        std::vector<T>& joints,
        std::vector<int>& parents,
        std::vector<std::string>& names);

}

#include "skeleton_io.cpp"

#endif // NVL_MODELS_SKELETON_IO_H
