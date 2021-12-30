#include "model_io.h"

#include <nvl/models/io/mesh_io.h>
#include <nvl/models/io/skeleton_io.h>
#include <nvl/models/io/animation_io.h>
#include <nvl/models/io/skinning_weights_io.h>

#include <nvl/models/io/model_io_rig.h>
#include <nvl/models/io/model_io_fbx.h>

#include <nvl/utilities/string_utils.h>

namespace nvl {

template<class M>
bool modelLoadFromFile(
        const std::string& filename,
        M& model,
        IOModelError& error,
        const IOModelMode& mode)
{
    IOModelData<
            typename M::Mesh,
            typename M::Skeleton,
            typename M::SkinningWeights,
            typename M::Animation> modelData;

    std::string ext = stringToLower(filenameExtension(filename));
    std::string path = filenamePath(filename);

    model.clear();

    bool success;
    if (ext == "rig") {
        success = modelLoadDataFromRIG(filename, modelData, error, mode);
    }
#ifdef NVL_FBXSDK_LOADED
    else if (ext == "fbx") {
        success = modelLoadDataFromFBX(filename, modelData, error, mode);
    }
#endif
    else if (ext == "obj") {
        IOMeshError meshError;
        success = meshLoadFromFile(filename, model.mesh, meshError, mode.meshMode);
        if (meshError != IOMeshError::IO_MESH_SUCCESS) {
            error = IOModelError::IO_MODEL_MESH_ERROR;
        }
    }
    else if (ext == "skt") {
        IOSkeletonError skeletonError;
        success = skeletonLoadFromFile(filename, model.skeleton, skeletonError, mode.skeletonMode);
        if (skeletonError != IOSkeletonError::IO_SKELETON_SUCCESS) {
            error = IOModelError::IO_MODEL_SKELETON_ERROR;
        }
    }
    else {
        error = IO_MODEL_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        modelLoadData(model, modelData, mode);
    }
    if (!success) {
        model.clear();
    }

    return success;
}

template<class M>
bool modelSaveToFile(
        const std::string& filename,
        const M& model,
        IOModelError& error,
        const IOModelMode& mode)
{

    std::string ext = stringToLower(filenameExtension(filename));
    std::string path = filenamePath(filename);
    std::string name = filenameName(filename);

    bool success;
    if (ext == "rig") {
        IOModelData<
                typename M::Mesh,
                typename M::Skeleton,
                typename M::SkinningWeights,
                typename M::Animation> modelData;

        modelSaveData(model, modelData, mode);

        success = modelSaveDataToRIG(filename, modelData, error, mode);
    }
#ifdef NVL_FBXSDK_LOADED
    else if (ext == "fbx") {
        IOModelData<
                typename M::Mesh,
                typename M::Skeleton,
                typename M::SkinningWeights,
                typename M::Animation> modelData;

        modelSaveData(model, modelData, mode);

        success = modelSaveDataToFBX(filename, modelData, error, mode);
    }
#endif
    else if (ext == "obj") {
        IOMeshError meshError;
        success = meshSaveToFile(filename, model.mesh, meshError, mode.meshMode);
        if (meshError != IOMeshError::IO_MESH_SUCCESS) {
            error = IOModelError::IO_MODEL_MESH_ERROR;
        }
    }
    else if (ext == "skt") {
        IOSkeletonError skeletonError;
        success = skeletonSaveToFile(filename, model.skeleton, skeletonError, mode.skeletonMode);
        if (skeletonError != IOSkeletonError::IO_SKELETON_SUCCESS) {
            error = IOModelError::IO_MODEL_SKELETON_ERROR;
        }
    }
    else {
        error = IO_MODEL_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}

template<class M, class MD>
void modelLoadData(
        M& model,
        const MD& modelData,
        const IOModelMode& mode)
{
    typedef typename M::Animation Animation;

    model.name = modelData.name;
    if (mode.mesh) {
        model.mesh = modelData.mesh;
    }
    if (mode.skeleton) {
        model.skeleton = modelData.skeleton;
    }
    if (mode.mesh && mode.skeleton) {
        model.skinningWeights = modelData.skinningWeights;
    }
    if (mode.animations && mode.skeleton) {
        for (const Animation& animation : modelData.animations) {
            model.addAnimation(animation);
        }
    }
}

template<class M, class MD>
void modelSaveData(
        const M& model,
        MD& modelData,
        const IOModelMode& mode)
{
    typedef typename M::Animation Animation;

    modelData.name = model.name;
    if (mode.mesh) {
        modelData.mesh = model.mesh;
    }
    if (mode.skeleton) {
        modelData.skeleton = model.skeleton;
    }
    if (mode.mesh && mode.skeleton) {
        modelData.skinningWeights = model.skinningWeights;
    }
    if (mode.animations && mode.skeleton) {
        for (const Animation& animation : model.animations) {
            modelData.animations.push_back(animation);
        }
    }
}


}
