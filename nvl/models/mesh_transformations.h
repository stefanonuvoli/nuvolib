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
