/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_IO_H
#define NVL_MODELS_MESH_IO_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/mesh_io_data.h>
#include <nvl/models/io/mesh_io_obj.h>

#include <nvl/models/structures/vertex_mesh.h>
#include <nvl/models/structures/polyline_mesh.h>
#include <nvl/models/structures/face_mesh.h>

namespace nvl {

template<class M>
bool meshLoadFromFile(
        const std::string& filename,
        M& mesh,
        IOMeshError& error = internal::dummyIOMeshError,
        const IOMeshMode& mode = internal::dummyIOMeshMode);

template<class M>
bool meshSaveToFile(
        const std::string& filename,
        const M& mesh,
        IOMeshError& error = internal::dummyIOMeshError,
        const IOMeshMode& mode = internal::dummyIOMeshMode);


/* Specialized load raw data IO */

template<class VT, class MD>
void meshLoadData(
        VertexMesh<VT>& mesh,
        const MD& meshData,
        const IOMeshMode& mode);

template<class VT, class PT, class MD>
void meshLoadData(
        PolylineMesh<VT,PT> mesh,
        const MD& meshData,
        const IOMeshMode& mode);

template<class VT, class PT, class FT, class MD>
void meshLoadData(
        FaceMesh<VT,PT,FT>& mesh,
        const MD& meshData,
        const IOMeshMode& mode);

/* Specialized save raw data IO */

template<class VT, class MD>
void meshSaveData(
        const VertexMesh<VT>& mesh,
        MD& meshData,
        const IOMeshMode& mode);

template<class VT, class PT, class MD>
void meshSaveData(
        const PolylineMesh<VT,PT> mesh,
        MD& meshData,
        const IOMeshMode& mode);

template<class VT, class PT, class FT, class MD>
void meshSaveData(
        const FaceMesh<VT,PT,FT>& mesh,
        MD& meshData,
        const IOMeshMode& mode);

/* Load raw data IO */

template<class M, class MD>
void meshLoadVertexData(
        M& mesh,
        const MD& meshData,
        const IOMeshMode& mode);

template<class M, class MD>
void meshLoadPolylineData(
        M& mesh,
        const MD& meshData,
        const IOMeshMode& mode);

template<class M, class MD>
void meshLoadFaceData(
        M& mesh,
        const MD& meshData,
        const IOMeshMode& mode);


/* Save raw data IO */

template<class M, class MD>
void meshSaveVertexData(
        M& mesh,
        MD& meshData,
        const IOMeshMode& mode);

template<class M, class MD>
void meshSavePolylineData(
        M& mesh,
        MD& meshData,
        const IOMeshMode& mode);

template<class M, class MD>
void meshSaveFaceData(
        M& mesh,
        MD& meshData,
        const IOMeshMode& mode);

}

#include "mesh_io.cpp"

#endif // NVL_MODELS_MESH_IO_H
