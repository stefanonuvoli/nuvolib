#ifndef NVL_MODELS_MESH_IO_OBJ_H
#define NVL_MODELS_MESH_IO_OBJ_H

#include <nvl/nuvolib.h>

#include <nvl/models/mesh_io_data.h>

#include <string>

namespace nvl {

template<class P, class VN, class UV, class VC, class PC, class FN, class M>
bool meshLoadDataFromObj(
        const std::string& filename,
        IOMeshData<P,VN,UV,VC,PC,FN,M>& data,
        IOMeshError& error = internal::dummyIOMeshError,
        IOMeshMode& mode = internal::dummyIOMeshMode);

template<class P, class VN, class UV, class VC, class PC, class FN, class M>
bool meshSaveDataToObj(
        const std::string& filename,
        const IOMeshData<P,VN,UV,VC,PC,FN,M>& data,
        IOMeshError& error = internal::dummyIOMeshError,
        const IOMeshMode& mode = internal::dummyIOMeshMode);

}

#include "mesh_io_obj.cpp"

#endif // NVL_MODELS_MESH_IO_OBJ_H
