#include "model_io.h"

#include <nvl/models/io/mesh_io.h>
#include <nvl/models/io/skeleton_io.h>
#include <nvl/models/io/animation_io.h>
#include <nvl/models/io/skinning_weights_io.h>

#include <nvl/models/io/model_io_rig.h>
#include <nvl/models/io/model_io_fbx.h>

namespace nvl {

template<class Model>
bool modelLoadFromFile(
        const std::string& filename,
        Model& model,
        IOModelError& error,
        IOModelMode& mode)
{
    std::string ext = filenameExtension(filename);
    std::string path = filenamePath(filename);

    model.clear();

    bool success;
    if (ext == "rig") {
        success = modelLoadModelFromRig(filename, model, error, mode);
    }
#ifdef NVL_FBXSDK_LOADED
    else if (ext == "fbx") {
        std::vector<Model> fbxModels;
        success = modelLoadModelsFromFBX(filename, fbxModels, error, mode);
        if (success && !fbxModels.empty()) {
            model = fbxModels[0];
        }
    }
#endif
    else {
        error = IO_MODEL_EXTENSION_NON_SUPPORTED;
        success = false;
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
        success = modelSaveModelToRig(filename, model, error, mode);
    }
    else {
        error = IO_MODEL_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}

template<class Model>
bool modelLoadFromFile(
        const std::string& filename,
        std::vector<Model>& models,
        IOModelError& error,
        IOModelMode& mode)
{
    std::vector<Model> modelData;

    std::string ext = filenameExtension(filename);

    models.clear();

    bool success;
    if (ext == "rig") {
        Model model;
        success = modelLoadFromFile(filename, model, error, mode);
        if (success) {
            models.push_back(model);
        }
    }
#ifdef NVL_FBXSDK_LOADED
    else if (ext == "fbx") {
        success = modelLoadDataFromFBX(filename, models, error, mode);
    }
#endif
    else {
        error = IO_MODEL_EXTENSION_NON_SUPPORTED;
        success = false;
    }


    return success;
}


}
