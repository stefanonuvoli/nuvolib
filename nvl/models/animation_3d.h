/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_ANIMATION_3D_H
#define NVL_MODELS_ANIMATION_3D_H

#include <nvl/nuvolib.h>

#include <nvl/models/structures/animation.h>

#include <nvl/math/affine.h>

namespace nvl {

typedef Animation<Affine3d> Animation3d;

}

#endif // NVL_MODELS_ANIMATION_3D_H
