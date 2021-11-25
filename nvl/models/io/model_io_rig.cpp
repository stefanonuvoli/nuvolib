#include "model_io_rig.h"

#include <nvl/utilities/file_utils.h>

#include <nvl/models/io/mesh_io.h>
#include <nvl/models/io/skeleton_io.h>
#include <nvl/models/io/skinning_weights_io.h>
#include <nvl/models/io/animation_io.h>

#include <fstream>

namespace nvl {

template<class M, class S, class W, class A>
bool modelLoadDataFromRIG(
        const std::string& filename,
        IOModelData<M,S,W,A>& modelData,
        IOModelError& error,
        const IOModelMode& mode)
{    
    //Get filename path
    std::string path = filenamePath(filename);

    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    std::ifstream fRig; //File streams
    std::string line;

    error = IO_MODEL_SUCCESS;

    //Open file
    fRig.open(filename);
    if(!fRig.is_open()) { //Error opening file
        error = IO_MODEL_FILE_ERROR;
        return false;
    }

    std::string name;

    std::string meshFile;
    std::string skeletonFile;
    std::string skinningWeightsFile;
    std::vector<std::string> animationFiles;

    while (std::getline(fRig, line)) {
        std::istringstream iss(line);

        std::string name;

        std::string token;
        iss >> token;

        if (token == "n") {
            bool first = true;
            while (!iss.eof()) {
                std::string tmpString;
                iss >> tmpString;
                if (!first)
                    name += " ";
                name += tmpString;
                first = false;
            }
        }
        if (token == "m") {
            if (!meshFile.empty()) {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
                return false;
            }

            bool first = true;
            while (!iss.eof()) {
                std::string tmpString;
                iss >> tmpString;
                if (!first)
                    meshFile += " ";
                meshFile += tmpString;
                first = false;
            }
            if (meshFile.at(0) != '/') {
                meshFile = path + meshFile;
            }
        }
        else if (token == "s") {
            if (!skeletonFile.empty()) {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
                return false;
            }

            bool first = true;
            while (!iss.eof()) {
                std::string tmpString;
                iss >> tmpString;
                if (!first)
                    skeletonFile += " ";
                skeletonFile += tmpString;
                first = false;
            }
            if (skeletonFile.at(0) != '/') {
                skeletonFile = path + skeletonFile;
            }
        }
        else if (token == "w") {
            if (!skinningWeightsFile.empty()) {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
                return false;
            }

            bool first = true;
            while (!iss.eof()) {
                std::string tmpString;
                iss >> tmpString;
                if (!first)
                    skinningWeightsFile += " ";
                skinningWeightsFile += tmpString;
                first = false;
            }
            if (skinningWeightsFile.at(0) != '/') {
                skinningWeightsFile = path + skinningWeightsFile;
            }
        }
        else if (token == "a") {
            std::string animationFilename;

            bool first = true;
            while (!iss.eof()) {
                std::string tmpString;
                iss >> tmpString;
                if (!first)
                    animationFilename += " ";
                animationFilename += tmpString;
                first = false;
            }
            if (animationFilename.at(0) != '/') {
                animationFilename = path + animationFilename;
            }

            animationFiles.push_back(animationFilename);
        }
    }

    if (name.empty()) {
        name = filenameName(filename);
    }

    fRig.close();

    bool success = true;
    if (success) {
        modelData.name = name;

        if (!meshFile.empty()) {
            IOMeshError meshError;
            success &= meshLoadFromFile(meshFile, modelData.mesh, meshError, mode.meshMode);
            if (!success) {
                error = IO_MODEL_MESH_ERROR;
            }
        }

        if (success && !skeletonFile.empty()) {
            IOSkeletonError skeletonError;
            success &= skeletonLoadFromFile(skeletonFile, modelData.skeleton, skeletonError, mode.skeletonMode);
            if (!success) {
                error = IO_MODEL_SKELETON_ERROR;
            }
        }

        if (success && !skinningWeightsFile.empty()) {
            modelData.skinningWeights.initialize(modelData.mesh.nextVertexId(), modelData.skeleton.jointNumber());

            IOSkinningWeightsError skinningWeightsError;
            success &= skinningWeightsLoadFromFile(skinningWeightsFile, modelData.skinningWeights, skinningWeightsError, mode.skinningWeightsMode);
            if (!success) {
                error = IO_MODEL_SKINNINGWEIGHTS_ERROR;
            }
        }

        if (success) {
            modelData.animations.resize(animationFiles.size());
            for (Index i = 0; i < animationFiles.size(); ++i) {
                IOAnimationError animationError;
                success &= animationLoadFromFile(animationFiles[i], modelData.animations[i], animationError, mode.animationMode);
                if (!success) {
                    error = IO_MODEL_ANIMATION_ERROR;
                }
            }
        }
    }

    return true;
}

template<class M, class S, class W, class A>
bool modelSaveDataToRIG(
        const std::string& filename,
        const IOModelData<M,S,W,A>& modelData,
        IOModelError& error,
        const IOModelMode& mode)
{
    //Get filename path
    std::string path = filenamePath(filename);

    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    std::ofstream fRig;

    error = IO_MODEL_SUCCESS;

    //Open file
    fRig.open(filename);
    if(!fRig.is_open()) { //Error opening file
        error = IO_MODEL_FILE_ERROR;
        return false;
    }

    std::string name = filenameName(filename);
    std::string meshFile = name + ".obj";
    std::string skeletonFile = name + ".skt";
    std::string skinningWeightsFile = name + ".skw";
    std::vector<std::string> animationFiles(modelData.animations.size());
    for (Index i = 0; i < modelData.animations.size(); ++i) {
        animationFiles[i] = modelData.animations[i].name() + ".ska";
    }

    bool success = true;
    if (mode.mesh) {
        IOMeshError meshError;
        success &= meshSaveToFile(path + meshFile, modelData.mesh, meshError, mode.meshMode);
        if (!success) {
            error = IO_MODEL_MESH_ERROR;
        }
    }

    if (success && mode.skeleton) {
        IOSkeletonError skeletonError;
        success &= skeletonSaveToFile(path + skeletonFile, modelData.skeleton, skeletonError, mode.skeletonMode);
        if (!success) {
            error = IO_MODEL_SKELETON_ERROR;
        }
    }

    if (success && mode.skinningWeights) {
        IOSkinningWeightsError skinningWeightsError;
        success &= skinningWeightsSaveToFile(path + skinningWeightsFile, modelData.skinningWeights, skinningWeightsError, mode.skinningWeightsMode);
        if (!success) {
            error = IO_MODEL_SKINNINGWEIGHTS_ERROR;
        }
    }

    if (success && mode.animations) {
        for (Index i = 0; i < modelData.animations.size(); ++i) {
            IOAnimationError animationError;
            success &= animationSaveToFile(path + animationFiles[i], modelData.animations[i], animationError, mode.animationMode);
            if (!success) {
                error = IO_MODEL_ANIMATION_ERROR;
            }
        }
    }

    if (success) {
        //Set precision
        fRig.precision(6);
        fRig.setf(std::ios::fixed, std:: ios::floatfield);

        if (!name.empty()) {
            fRig << "n " << name << std::endl;
        }
        if (mode.mesh) {
            fRig << "m " << meshFile << std::endl;
        }
        if (mode.skeleton) {
            fRig << "s " << skeletonFile << std::endl;
        }
        if (mode.skinningWeights) {
            fRig << "w " << skinningWeightsFile << std::endl;
        }
        if (mode.animations) {
            for (const std::string& s : animationFiles)
            fRig << "a " << s << std::endl;
        }
    }

    //Close obj file
    fRig.close();

    return success;
}

}
