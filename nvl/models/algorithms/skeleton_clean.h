/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
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
