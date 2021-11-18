#include "model_io_rig.h"

#include <nvl/utilities/file_utils.h>

#include <fstream>

namespace nvl {

NVL_INLINE bool modelLoadDataFromRig(
        const std::string& filename,
        IOModelData& data,
        IOModelError& error,
        IOModelMode& mode)
{
    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    data.clear();

    std::ifstream fRig; //File streams
    std::string line;

    error = IO_MODEL_SUCCESS;

    //Open file
    fRig.open(filename);
    if(!fRig.is_open()) { //Error opening file
        error = IO_MODEL_FILE_ERROR;
        return false;
    }

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
                    data.name += " ";
                data.name += tmpString;
                first = false;
            }
        }
        if (token == "m" && mode.mesh) {
            if (!data.meshFilename.empty()) {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
                return false;
            }

            bool first = true;
            while (!iss.eof()) {
                std::string tmpString;
                iss >> tmpString;
                if (!first)
                    data.meshFilename += " ";
                data.meshFilename += tmpString;
                first = false;
            }
        }
        else if (token == "s" && mode.skeleton) {
            if (!data.skeletonFilename.empty()) {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
                return false;
            }

            bool first = true;
            while (!iss.eof()) {
                std::string tmpString;
                iss >> tmpString;
                if (!first)
                    data.skeletonFilename += " ";
                data.skeletonFilename += tmpString;
                first = false;
            }
        }
        else if (token == "w" && mode.skinningWeights) {
            if (!data.skinningWeightsFilename.empty()) {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
                return false;
            }

            bool first = true;
            while (!iss.eof()) {
                std::string tmpString;
                iss >> tmpString;
                if (!first)
                    data.skinningWeightsFilename += " ";
                data.skinningWeightsFilename += tmpString;
                first = false;
            }
        }
        else if (token == "a" && mode.animations) {
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

            data.animationFilenames.push_back(animationFilename);
        }
    }

    if (data.name.empty()) {
        std::string name = filenameName(filename);
        data.name = name;
    }

    fRig.close();

    //Successfully loaded
    return true;
}

NVL_INLINE bool modelSaveDataToRig(
        const std::string& filename,
        const IOModelData& data,
        IOModelError& error,
        const IOModelMode& mode)
{
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

    //Set precision
    fRig.precision(6);
    fRig.setf(std::ios::fixed, std:: ios::floatfield);

    if (!data.name.empty()) {
        fRig << "n " << data.name << std::endl;
    }
    if (mode.mesh) {
        fRig << "m " << data.meshFilename << std::endl;
    }
    if (mode.skeleton) {
        fRig << "s " << data.skeletonFilename << std::endl;
    }
    if (mode.skinningWeights) {
        fRig << "w " << data.skinningWeightsFilename << std::endl;
    }
    if (mode.animations) {
        for (const std::string& s : data.animationFilenames)
        fRig << "a " << s << std::endl;
    }

    //Close obj file
    fRig.close();

    return true;
}

}
