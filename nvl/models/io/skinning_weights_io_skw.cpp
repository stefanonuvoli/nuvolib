/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "skinning_weights_io_skw.h"

#include <nvl/utilities/file_utils.h>
#include <nvl/utilities/locale_utils.h>

#include <fstream>

namespace nvl {

/**
 * @brief Load skinning weights from SKW file
 * @param filename Filename
 * @param skinningWeights Skinning weights
 * @param error Error
 * @return True if loading has been successful
 */
template<class T>
bool skinningWeightsLoadDataFromSKW(
        const std::string& filename,
        IOSkinningWeightsData<T>& data,
        IOSkinningWeightsError& error)
{
    data.clear();

    std::ifstream fSkw; //File streams

    fSkw.imbue(streamDefaultLocale());

    std::string line;

    error = IO_SKINNINGWEIGHTS_SUCCESS;

    //Open file
    fSkw.open(filename);
    if(!fSkw.is_open()) { //Error opening file
        error = IO_SKINNINGWEIGHTS_FILE_ERROR;
        return false;
    }

    while (std::getline(fSkw, line)) {
        if (line.empty() || line.at(0) == '#')
            continue;

        std::istringstream iss(line);

        int j, v;
        T w;

        iss >> j >> v >> w;

        data.weights.push_back(std::make_tuple(v, j, w));
    }

    fSkw.close();

    //Successfully loaded
    return true;
}

/**
 * @brief Save skinning weights to SKW file
 * @param filename Filename
 * @param skinningWeights Skinning weights
 * @param error Error
 * @return True if saving has been successful
 */
template<class T>
bool skinningWeightsSaveDataToSKW(
        const std::string& filename,
        const IOSkinningWeightsData<T>& data,
        IOSkinningWeightsError& error)
{
    std::ofstream fSkw;

    fSkw.imbue(streamDefaultLocale());

    error = IO_SKINNINGWEIGHTS_SUCCESS;

    //Open file
    fSkw.open(filename);
    if(!fSkw.is_open()) { //Error opening file
        error = IO_SKINNINGWEIGHTS_FILE_ERROR;
        return false;
    }    

    fSkw <<
            "###############################" << std::endl <<
            "# Generated by nuvolib (SKA)" << std::endl <<
            "# Animation SKA file" << std::endl <<
            "# Number of weights: " << data.weights.size() << std::endl <<
            "###############################" << std::endl
            << std::endl;

    //Set precision
    fSkw.precision(6);
    fSkw.setf(std::ios::fixed, std::ios::floatfield);

    for (const std::tuple<Index, Index, T>& tuple : data.weights) {
        const Index& v = std::get<0>(tuple);
        const Index& j = std::get<1>(tuple);
        const T& w = std::get<2>(tuple);

        fSkw << j << " " << v << " " << w << std::endl;
    }

    //Successfully loaded
    return true;
}

}
