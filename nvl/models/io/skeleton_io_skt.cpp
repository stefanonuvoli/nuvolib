#include "skeleton_io_skt.h"

#include <nvl/utilities/locale_utils.h>

#include <nvl/math/translation.h>
#include <nvl/math/rotation.h>
#include <nvl/math/euler_angles.h>
#include <nvl/math/comparisons.h>

#include <fstream>
#include <regex>

namespace nvl {

template<class V>
bool skeletonLoadDataFromSKT(
        const std::string& filename,
        IOSkeletonData<V>& data,
        IOSkeletonError& error)
{
    data.clear();

    std::ifstream fSkt; //File streams

    fSkt.imbue(streamDefaultLocale());

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

        if (token[0] == 'j') {
           int i, father;
           std::string name;
           bool hidden = false;

           iss >> i;

           std::string currentString;
           bool isNumeric;
           do {
               iss >> currentString;
               isNumeric = std::regex_match(currentString, std::regex("(\\+|-)?[[:digit:]]+"));

               if (!isNumeric) {
                   if (!name.empty()) {
                       name += ' ';
                   }
                   name += currentString;
               }
           } while (!isNumeric);

           father = std::stoi(currentString);

           Affine3d transformation;

           double t1, t2, t3;
           double r1, r2, r3;
           iss >> t1 >> t2 >> t3;
           iss >> r1 >> r2 >> r3;

           Vector3d ang(r1, r2, r3);
           for (EigenId i = 0; i < ang.size(); ++i) {
               ang[i] = ang[i] / 180.0 *  M_PI;
           }

           Rotation3d rot = eulerAnglesToRotationXYZ(ang);
           Translation3d tra(t1, t2, t3);

           transformation = tra * rot;

           if (!iss.eof()) {
               std::string hiddenString;
               iss >> hiddenString;
               if (hiddenString == "hidden") {
                   hidden = true;
               }
           }

           data.joints.push_back(transformation);
           data.hidden.push_back(hidden);
           data.parents.push_back(father);
           data.names.push_back(name);
        }
    }

    fSkt.close();

    //Successfully loaded
    return true;
}

template<class V>
bool skeletonSaveDataToSKT(
        const std::string& filename,
        const IOSkeletonData<V>& data,
        IOSkeletonError& error)
{
    std::ofstream fSkt;

    fSkt.imbue(streamDefaultLocale());

    error = IO_SKELETON_SUCCESS;

    //Open file
    fSkt.open(filename);
    if(!fSkt.is_open()) { //Error opening file
        error = IO_SKELETON_FILE_ERROR;
        return false;
    }
    fSkt <<
        "###############################" << std::endl <<
        "# Generated by nuvolib (SKT)" << std::endl <<
        "# Skeleton SKT file" << std::endl <<
        "# Joints: " << data.joints.size() << std::endl <<
        "###############################" << std::endl <<
        std::endl;

    //Set precision
    fSkt.precision(6);
    fSkt.setf(std::ios::fixed, std::ios::floatfield);

    for (Index i = 0; i < data.joints.size(); ++i) {
        const V& t = data.joints[i];
        const bool& h = data.hidden[i];
        const int& p = data.parents[i];
        const std::string& n = data.names[i];

        Translation3d tra(t.translation());
        Rotation3d rot(t.rotation());

        Vector3d ang = eulerAnglesFromRotationXYZ(rot);

        for (EigenId i = 0; i < ang.size(); ++i) {
            ang[i] = ang[i] / M_PI * 180.0;
        }

        fSkt << "j " << i << " " << n << " " << p;
        fSkt << " " << tra.x() << " " << tra.y() << " " << tra.z();
        fSkt << " " << ang.x() << " " << ang.y() << " " << ang.z();

        if (h) {
            fSkt << " " << "hidden";
        }

        fSkt << std::endl;
    }

    //Successfully loaded
    return true;
}

}
