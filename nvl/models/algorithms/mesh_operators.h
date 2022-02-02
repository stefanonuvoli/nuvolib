/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_OPERATORS_H
#define NVL_MODELS_MESH_OPERATORS_H

#include <nvl/nuvolib.h>

#include <nvl/math/vector.h>

#include <vector>

namespace nvl {

template<class Mesh, class R = typename Mesh::Scalar>
std::vector<Vector3<R>> meshLaplaceOperator(
        const Mesh& mesh);

template<class Mesh, class R = typename Mesh::Scalar>
std::vector<Vector3<R>> meshLaplaceOperator(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);

template<class Mesh, class R = typename Mesh::Scalar>
Vector3<R> meshLaplaceOperator(
        const Mesh& mesh,
        typename Mesh::VertexId& vId);

template<class Mesh, class R = typename Mesh::Scalar>
Vector3<R> meshLaplaceOperator(
        const Mesh& mesh,
        typename Mesh::VertexId& vId,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj);

template<class Mesh, class R = typename Mesh::Scalar>
std::vector<Vector3<R>> meshLaplaceBeltramiOperator(
        const Mesh& mesh);

template<class Mesh, class R = typename Mesh::Scalar>
std::vector<Vector3<R>> meshLaplaceBeltramiOperator(
        const Mesh& mesh,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);

template<class Mesh, class R = typename Mesh::Scalar>
Vector3<R> meshLaplaceBeltramiOperator(
        const Mesh& mesh,
        typename Mesh::VertexId& vId);

template<class Mesh, class R = typename Mesh::Scalar>
Vector3<R> meshLaplaceBeltramiOperator(
        const Mesh& mesh,
        typename Mesh::VertexId& vId,
        const std::vector<std::vector<typename Mesh::VertexId>>& vvAdj,
        const std::vector<std::vector<typename Mesh::FaceId>>& vfAdj);

}

#include "mesh_operators.cpp"

#endif // NVL_MODELS_MESH_OPERATORS_H
