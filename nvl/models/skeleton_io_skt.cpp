#include "skeleton_io_skt.h"

#include <nvl/math/translation.h>
#include <nvl/math/rotation.h>
#include <nvl/math/euler_angles.h>
#include <nvl/math/comparisons.h>

#include <fstream>
#include <regex>

namespace nvl {

template<class V>
bool skeletonLoadDataFromSkt(
        const std::string& filename,
        IOSkeletonData<V>& data,
        IOSkeletonError& error,
        IOSkeletonMode& mode)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mode);

    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    data.clear();

    std::ifstream fSkt; //File streams
    std::string line;

    error = IO_SKELETON_SUCCESS;

    //Open file
    fSkt.open(filename);
    if(!fSkt.is_open()) { //Error opening file
        error = IO_SKELETON_FILE_ERROR;
        return false;
    }

    while (std::getline(fSkt, line)) {
        std::istringstream iss(line);

        std::string token;
        iss >> token;
        if (token.size() > 1)
            continue;

        if (token[0] == 'j')
        {
           int i, father;
           std::string name;

           iss >> i;

           std::string fatherString;
           bool isNumeric;
           do {
               iss >> fatherString;
               isNumeric = std::regex_match(fatherString, std::regex("(\\+|-)?[[:digit:]]+"));
               if (!isNumeric) {
                   if (!name.empty()) {
                       name += " ";
                   }
                   name += fatherString;
               }
           } while (!isNumeric);

           father = std::stoi(fatherString);

           Affine3d transformation;

           double x1, y1, z1;
           iss >> x1 >> y1 >> z1;
           if (iss.eof()) {
               transformation = Translation3d(x1, y1, z1);
           }
           else {
               double x2, y2, z2;
               iss >> x2 >> y2 >> z2;

               Vector3d eulerAngles(x1, y1, z1);
               for (EigenId i = 0; i < eulerAngles.size(); ++i) {
                   eulerAngles[i] = eulerAngles[i] / 180.0 *  M_PI;
               }
               Rotation3d rotation = eulerAnglesToRotationXYZ(eulerAngles);

               transformation = Translation3d(x2, y2, z2) * rotation;
           }

           data.joints.push_back(transformation);
           data.parents.push_back(father);
           data.names.push_back(name);
        }
    }

    fSkt.close();

    //Successfully loaded
    return true;
}

template<class V>
bool skeletonSaveDataToSkt(
        const std::string& filename,
        const IOSkeletonData<V>& data,
        IOSkeletonError& error,
        const IOSkeletonMode& mode)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(mode);

    //Use "." as decimal separator
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    std::ofstream fSkt;

    error = IO_SKELETON_SUCCESS;

    //Open file
    fSkt.open(filename);
    if(!fSkt.is_open()) { //Error opening file
        error = IO_SKELETON_FILE_ERROR;
        return false;
    }

    //Set precision
    fSkt.precision(6);
    fSkt.setf(std::ios::fixed, std:: ios::floatfield);

    for (Index i = 0; i < data.joints.size(); ++i) {
        const V& t = data.joints[i];
        const int& p = data.parents[i];
        const std::string& n = data.names[i];

        Translation3d tra(t.translation());
        Rotation3d rot(t.rotation());

        Vector3d eulerAngles = eulerAnglesFromRotationXYZ(rot);

        for (EigenId i = 0; i < eulerAngles.size(); ++i) {
            eulerAngles[i] = eulerAngles[i] / M_PI * 180.0;
        }

        fSkt << "j " << i << " " << n << " " << p;

        if (!epsEqual(eulerAngles, Vector3d(0,0,0))) {
            fSkt << " " << eulerAngles.x() << " " << eulerAngles.y() << " " << eulerAngles.z();
        }

        fSkt << " " << tra.x() << " " << tra.y() << " " << tra.z() << std::endl;
    }

    //Successfully loaded
    return true;
}

}
