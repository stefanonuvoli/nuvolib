/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_TRANSFORMATIONS_H
#define NVL_MODELS_MESH_TRANSFORMATIONS_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

namespace nvl {

template<class Mesh, class T>
void meshApplyTransformation(Mesh& mesh, const T& transformation);
template<class Mesh, class T>
void meshApplyTransformation(Mesh& mesh, const Matrix44<T>& transformation);

}

#include "mesh_transformations.cpp"

#endif // NVL_MODELS_MESH_TRANSFORMATIONS_H
