#include "skinning_weights_io_skw.h"

#include <nvl/utilities/file_utils.h>

#include <fstream>

namespace nvl {

template<class T>
bool skinningWeightsLoadDataFromSKW(
        const std::string& filename,
        IOSkinningWeightsData<T>& data,
        IOSkinningWeightsError& error)
{
    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    data.clear();

    std::ifstream fSkw; //File streams
    std::string line;

    error = IO_SKINNINGWEIGHTS_SUCCESS;

    //Open file
    fSkw.open(filename);
    if(!fSkw.is_open()) { //Error opening file
        error = IO_SKINNINGWEIGHTS_FILE_ERROR;
        return false;
    }

    while (std::getline(fSkw, line)) {
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

template<class T>
bool skinningWeightsSaveDataToSKW(
        const std::string& filename,
        const IOSkinningWeightsData<T>& data,
        IOSkinningWeightsError& error)
{
    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    std::ofstream fSkw;

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
