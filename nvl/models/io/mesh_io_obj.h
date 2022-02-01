/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MESH_IO_OBJ_H
#define NVL_MODELS_MESH_IO_OBJ_H

#include <nvl/nuvolib.h>

#include <nvl/models/io/mesh_io_data.h>

#include <string>

namespace nvl {

template<class P, class VN, class UV, class VC, class PC, class FN, class M>
bool meshLoadDataFromOBJ(
        const std::string& filename,
        IOMeshData<P,VN,UV,VC,PC,FN,M>& data,
        IOMeshError& error);

template<class P, class VN, class UV, class VC, class PC, class FN, class M>
bool meshSaveDataToOBJ(
        const std::string& filename,
        const IOMeshData<P,VN,UV,VC,PC,FN,M>& data,
        IOMeshError& error);

}

#include "mesh_io_obj.cpp"

#endif // NVL_MODELS_MESH_IO_OBJ_H
