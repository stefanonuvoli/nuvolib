#include "animation_io_ska.h"

#include <nvl/math/matrix.h>
#include <nvl/math/translation.h>
#include <nvl/math/rotation.h>
#include <nvl/math/euler_angles.h>
#include <nvl/math/comparisons.h>

#include <nvl/utilities/file_utils.h>

#include <fstream>

namespace nvl {

template<class T>
bool animationLoadDataFromSKA(
        const std::string& filename,
        IOAnimationData<T>& data,
        IOAnimationError& error)
{
    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    data.clear();

    std::ifstream fSka; //File streams
    std::string line;

    error = IO_ANIMATION_SUCCESS;

    //Open file
    fSka.open(filename);
    if(!fSka.is_open()) { //Error opening file
        error = IO_ANIMATION_FILE_ERROR;
        return false;
    }

    while (std::getline(fSka, line)) {
        std::istringstream iss(line);

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
        else if (token == "n") {
            data.name.clear();
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
        else if (token[0] == 'k') {
            double time;
            iss >> time;

            data.times.push_back(time);
            data.transformations.push_back(std::vector<T>());
        }
        else if (token[0] == 's') {
            int index;

            Vector3d eulerAngles;
            Vector3d translateVector;

            iss >>
                  index >>
                  eulerAngles(0) >>
                  eulerAngles(1) >>
                  eulerAngles(2) >>
                  translateVector(0) >>
                  translateVector(1) >>
                  translateVector(2);

            for (EigenId i = 0; i < eulerAngles.size(); ++i) {
                eulerAngles[i] = eulerAngles[i] / 180.0 *  M_PI;
            }

            Translation3d translation(translateVector);
            Rotation3d rotation = eulerAnglesToRotationXYZ(eulerAngles);

            T transformation(translation * rotation);

            data.transformations[index].push_back(transformation);
        }
    }

    if (data.name.empty()) {
        std::string name = filenameName(filename);
        data.name = name;
    }

    fSka.close();

    //Successfully loaded
    return true;
}

template<class T>
bool animationSaveDataToSKA(
        const std::string& filename,
        const IOAnimationData<T>& data,
        IOAnimationError& error)
{
    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    std::ofstream fSka;

    error = IO_ANIMATION_SUCCESS;

    //Open file
    fSka.open(filename);
    if(!fSka.is_open()) { //Error opening file
        error = IO_ANIMATION_FILE_ERROR;
        return false;
    }

    //Set precision
    fSka.precision(6);
    fSka.setf(std::ios::fixed, std:: ios::floatfield);


    for (const double& t : data.times) {
        fSka << "k " << t << std::endl;
    }

    for (Index i = 0; i < data.transformations.size(); ++i) {
        const std::vector<T>& frameTransformations = data.transformations[i];

        for (const T& t : frameTransformations) {
            Rotation3d rot(t.rotation());
            Vector3d tra(t.translation());

            const double eps = 1e-5;

            Vector3d eulerAngles = eulerAnglesFromRotationXYZ(rot);
            for (EigenId i = 0; i < eulerAngles.size(); ++i) {
                eulerAngles[i] = eulerAngles[i] / M_PI * 180.0;

                if (epsEqual(eulerAngles[i], 0.0, eps)) {
                    eulerAngles[i] = 0.0;
                }
            }

            for (EigenId i = 0; i < tra.size(); ++i) {
                if (epsEqual(tra[i], 0.0, eps)) {
                    tra[i] = 0.0;
                }
            }

            fSka << "s " << i << " " << eulerAngles.x() << " " << eulerAngles.y() << " " << eulerAngles.z() << " " << tra.x() << " " << tra.y() << " " << tra.z() << std::endl;
        }
    }

    //Successfully loaded
    return true;
}

}
