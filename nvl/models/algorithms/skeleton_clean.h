#ifndef NVL_MODELS_SKELETON_CLEAN_H
#define NVL_MODELS_SKELETON_CLEAN_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class S>
void skeletonRemoveScaling(S& skeleton);

}

#include "skeleton_clean.cpp"

#endif // NVL_MODELS_SKELETON_CLEAN_H
