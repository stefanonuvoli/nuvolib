#include "model_io.h"

#include <nvl/models/io/mesh_io.h>
#include <nvl/models/io/skeleton_io.h>
#include <nvl/models/io/animation_io.h>
#include <nvl/models/io/skinning_weights_io.h>

#include <nvl/models/io/model_io_rig.h>

namespace nvl {

template<class Model>
bool modelLoadFromFile(
        const std::string& filename,
        Model& model,
        IOModelError& error,
        IOModelMode& mode)
{
    IOModelData modelData;

    std::string ext = filenameExtension(filename);
    std::string path = filenamePath(filename);

    model.clear();

    bool success;
    if (ext == "rig") {
        success = modelLoadDataFromRig(filename, modelData, error, mode);
    }
    else {
        error = IO_MODEL_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        model.setName(modelData.name);

        if (mode.mesh && !modelData.meshFilename.empty()) {
            IOMeshError meshError;
            success &= meshLoadFromFile(path + modelData.meshFilename, model.mesh, meshError, mode.meshMode);
            if (!success) {
                error = IO_MODEL_MESH_ERROR;
            }
        }

        if (success && mode.skeleton && !modelData.skeletonFilename.empty()) {
            IOSkeletonError skeletonError;
            success &= skeletonLoadFromFile(path + modelData.skeletonFilename, model.skeleton, skeletonError, mode.skeletonMode);
            if (!success) {
                error = IO_MODEL_SKELETON_ERROR;
            }
        }

        if (success && mode.skinningWeights && !modelData.skinningWeightsFilename.empty()) {            
            model.initializeSkinningWeights();

            IOSkinningWeightsError skinningWeightsError;
            success &= skinningWeightsLoadFromFile(path + modelData.skinningWeightsFilename, model.skinningWeights, skinningWeightsError, mode.skinningWeightsMode);
        }

        if (success && mode.animations) {
            for (const std::string& animationFilename : modelData.animationFilenames) {
                typename Model::Animation animation;

                IOAnimationError animationError;
                success &= animationLoadFromFile(path + animationFilename, animation, animationError, mode.animationMode);

                model.addAnimation(animation);
            }
        }
    }
    if (!success) {
        model.clear();
    }

    return success;
}

template<class Model>
bool modelSaveToFile(
        const std::string& filename,
        const Model& model,
        IOModelError& error,
        const IOModelMode& mode)
{
    std::string ext = filenameExtension(filename);
    std::string path = filenamePath(filename);
    std::string name = filenameName(filename);

    bool success;
    if (ext == "rig") {
        IOModelData modelData;
        modelData.name = model.name();
        modelData.meshFilename = name + ".obj";
        modelData.skeletonFilename = name + ".skt";
        modelData.skinningWeightsFilename = name + ".skw";
        modelData.animationFilenames.resize(model.animationNumber());
        for (Index i = 0; i < model.animationNumber(); ++i) {
            modelData.animationFilenames[i] = model.animation(i).name() + ".ska";
        }

        success = true;

        if (mode.mesh) {
            IOMeshError meshError;
            success &= meshSaveToFile(path + modelData.meshFilename, model.mesh, meshError, mode.meshMode);
            if (!success) {
                error = IO_MODEL_MESH_ERROR;
            }
        }

        if (success && mode.skeleton) {
            IOSkeletonError skeletonError;
            success &= skeletonSaveToFile(path + modelData.skeletonFilename, model.skeleton, skeletonError, mode.skeletonMode);
            if (!success) {
                error = IO_MODEL_SKELETON_ERROR;
            }
        }

        if (success && mode.skinningWeights) {
            IOSkinningWeightsError skinningWeightsError;
            success &= skinningWeightsSaveToFile(path + modelData.skinningWeightsFilename, model.skinningWeights, skinningWeightsError, mode.skinningWeightsMode);
        }

        if (success && mode.animations) {
            for (Index i = 0; i < model.animationNumber(); ++i) {
                const std::string& animationFilename = modelData.animationFilenames[i];

                IOAnimationError animationError;
                success &= animationSaveToFile(path + animationFilename, model.animation(i), animationError, mode.animationMode);
            }
        }

        if (success) {
            success = modelSaveDataToRig(filename, modelData, error, mode);
        }
    }
    else {
        error = IO_MODEL_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}

}
