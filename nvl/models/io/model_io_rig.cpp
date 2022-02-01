/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "model_io_rig.h"

#include <nvl/utilities/file_utils.h>
#include <nvl/utilities/locale_utils.h>

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

    std::ifstream fRig; //File streams

    fRig.imbue(streamDefaultLocale());

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
        if (line.empty())
            continue;

        std::vector<std::string> lineSplitted = stringSplit(line, ' ');

        const std::string& token = lineSplitted[0];

        if (token == "n") {
            name = stringTrim(stringUnion(lineSplitted, 1));
        }
        else if (token == "m") {
            if (!meshFile.empty()) {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
                return false;
            }

            meshFile = stringTrim(stringUnion(lineSplitted, 1));
            meshFile = filenameAbsolutePath(meshFile, path);
        }
        else if (token == "s") {
            if (!skeletonFile.empty()) {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
                return false;
            }

            skeletonFile = stringTrim(stringUnion(lineSplitted, 1));
            skeletonFile = filenameAbsolutePath(skeletonFile, path);
        }
        else if (token == "w") {
            if (!skinningWeightsFile.empty()) {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
                return false;
            }

            skinningWeightsFile = stringTrim(stringUnion(lineSplitted, 1));
            skinningWeightsFile = filenameAbsolutePath(skinningWeightsFile, path);
        }
        else if (token == "a") {
            std::string animationFilename;

            animationFilename = stringTrim(stringUnion(lineSplitted, 1));
            animationFilename = filenameAbsolutePath(animationFilename, path);

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
            #pragma omp parallel for
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

    std::ofstream fRig;

    fRig.imbue(streamDefaultLocale());

    error = IO_MODEL_SUCCESS;

    //Open file
    fRig.open(filename);
    if(!fRig.is_open()) { //Error opening file
        error = IO_MODEL_FILE_ERROR;
        return false;
    }

    //Get name

    std::string name = filenameName(filename);

    std::string modelName = modelData.name;
    if (modelName.empty()) {
        modelName = name;
    }

    //Get paths of the files

    std::string componentPath;
    if (mode.RIGComponentFolder) {
        componentPath = name + "_rig/";
        nvl::createDirectory(filenameAbsolutePath(componentPath, path));
    }

    const std::string meshFileRel = componentPath + name + ".obj";
    const std::string meshFileAbs = filenameAbsolutePath(meshFileRel, path);

    const std::string skeletonFileRel = componentPath + name + ".skt";
    const std::string skeletonFileAbs = filenameAbsolutePath(skeletonFileRel, path);

    const std::string skinningWeightsFileRel = componentPath + name + ".skw";
    const std::string skinningWeightsFileAbs = filenameAbsolutePath(skinningWeightsFileRel, path);

    std::vector<std::string> animationFilesRel(modelData.animations.size());
    std::vector<std::string> animationFilesAbs(modelData.animations.size());

    const std::string animationFolderRel(componentPath + "animations/");
    const std::string animationFolderAbs = filenameAbsolutePath(animationFolderRel, path);
    for (Index i = 0; i < modelData.animations.size(); ++i) {
        animationFilesRel[i] =
                animationFolderRel +
                name + "@" +
                modelData.animations[i].name() +
                ".ska";

        animationFilesAbs[i] = filenameAbsolutePath(animationFilesRel[i], path);
    }

    //Save components
    bool success = true;
    if (mode.mesh) {
        IOMeshError meshError;

        success &= meshSaveToFile(
                meshFileAbs,
                modelData.mesh,
                meshError,
                mode.meshMode);

        if (!success) {
            error = IO_MODEL_MESH_ERROR;
        }
    }

    if (success && mode.skeleton) {
        IOSkeletonError skeletonError;

        success &= skeletonSaveToFile(
                skeletonFileAbs,
                modelData.skeleton,
                skeletonError,
                mode.skeletonMode);

        if (!success) {
            error = IO_MODEL_SKELETON_ERROR;
        }
    }

    if (success && mode.skinningWeights) {
        IOSkinningWeightsError skinningWeightsError;

        success &= skinningWeightsSaveToFile(
                    skinningWeightsFileAbs,
                    modelData.skinningWeights,
                    skinningWeightsError,
                    mode.skinningWeightsMode);

        if (!success) {
            error = IO_MODEL_SKINNINGWEIGHTS_ERROR;
        }
    }

    if (success && mode.animations) {
        if (!modelData.animations.empty())
            nvl::createDirectory(animationFolderAbs);

        for (Index i = 0; i < modelData.animations.size(); ++i) {
            IOAnimationError animationError;

            success &= animationSaveToFile(
                    animationFilesAbs[i],
                    modelData.animations[i],
                    animationError,
                    mode.animationMode);

            if (!success) {
                error = IO_MODEL_ANIMATION_ERROR;
            }
        }
    }

    //Create RIG
    if (success) {
        //Set precision
        fRig.precision(6);
        fRig.setf(std::ios::fixed, std::ios::floatfield);

        if (!modelName.empty()) {
            fRig << "n " << modelName << std::endl;
        }
        if (mode.mesh) {
            fRig << "m " << meshFileRel << std::endl;
        }
        if (mode.skeleton) {
            fRig << "s " << skeletonFileRel << std::endl;
        }
        if (mode.skinningWeights) {
            fRig << "w " << skinningWeightsFileRel << std::endl;
        }
        if (mode.animations) {
            for (const std::string& s : animationFilesRel) {
                fRig << "a " << s << std::endl;
            }
        }
    }

    //Close obj file
    fRig.close();

    return success;
}

}
