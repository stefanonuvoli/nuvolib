/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_MORPHOLOGICAL_OPERATIONS_H
#define NVL_MODELS_MESH_MORPHOLOGICAL_OPERATIONS_H

#include <nvl/nuvolib.h>

#include <vector>

namespace nvl {

template<class Mesh, class Set>
void meshDilateFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces);

template<class Mesh, class Set>
void meshDilateFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj);

template<class Mesh, class Set>
void meshErodeFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const bool fixBorders = false);

template<class Mesh, class Set>
void meshErodeFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        const bool fixBorders = false);

template<class Mesh, class Set>
void meshOpenFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const bool fixBorders = false);

template<class Mesh, class Set>
void meshOpenFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        const bool fixBorders = false);

template<class Mesh, class Set>
void meshCloseFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const bool fixBorders = false);

template<class Mesh, class Set>
void meshCloseFaceSelection(
        const Mesh& mesh,
        Set& selectedFaces,
        const std::vector<std::vector<typename Mesh::FaceId>>& ffAdj,
        const bool fixBorders = false);

}

#include "mesh_morphological_operations.cpp"

#endif // NVL_MODELS_MESH_MORPHOLOGICAL_OPERATIONS_H
