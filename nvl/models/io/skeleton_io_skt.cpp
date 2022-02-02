/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "skeleton_io_skt.h"

#include <nvl/utilities/locale_utils.h>
#include <nvl/utilities/string_utils.h>

#include <nvl/math/transformations.h>
#include <nvl/math/euler_angles.h>
#include <nvl/math/comparisons.h>

#include <fstream>
#include <regex>

namespace nvl {

template<class T>
bool skeletonLoadDataFromSKT(
        const std::string& filename,
        IOSkeletonData<T>& data,
        IOSkeletonError& error)
{
    typedef typename T::Scalar Scalar;

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
               isNumeric = stringIsNumeric(currentString);

               if (!isNumeric) {
                   if (!name.empty()) {
                       name += ' ';
                   }
                   name += currentString;
               }
           } while (!isNumeric);

           father = stringConvert<int>(currentString);

           double t1, t2, t3;
           double r1 = 0.0, r2 = 0.0, r3 = 0.0;
           double s1 = 1.0, s2 = 1.0, s3 = 1.0;

           iss >> t1 >> t2 >> t3;

           if (!iss.eof()) {
               iss >> currentString;

               bool isFloat = stringIsFloat(currentString);

               if (isFloat) {
                   r1 = stringConvert<double>(currentString);
                   iss >> r2 >> r3;
               }
               else if (currentString == "hidden") {
                   hidden = true;
               }
           }

           if (!iss.eof()) {
               iss >> currentString;

               bool isFloat = stringIsFloat(currentString);

               if (isFloat) {
                   s1 = stringConvert<double>(currentString);
                   iss >> s2 >> s3;
               }
               else if (currentString == "hidden") {
                   hidden = true;
               }
           }

           if (!iss.eof()) {
               iss >> currentString;

               if (currentString == "hidden") {
                   hidden = true;
               }
           }

           Vector3<Scalar> ang(r1, r2, r3);
           for (EigenId i = 0; i < ang.size(); ++i) {
               ang[i] = ang[i] / 180.0 *  M_PI;
           }

           Rotation3<Scalar> rot = eulerAnglesToRotationXYZ(ang);
           Translation3<Scalar> tra(t1, t2, t3);
           Scaling3<Scalar> sca(s1, s2, s3);

           T transformation = tra * rot * sca;

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

template<class T>
bool skeletonSaveDataToSKT(
        const std::string& filename,
        const IOSkeletonData<T>& data,
        IOSkeletonError& error)
{
    typedef typename T::LinearMatrixType LinearMatrixType;
    typedef typename T::Scalar Scalar;

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
        const T& t = data.joints[i];
        const bool& h = data.hidden[i];
        const int& p = data.parents[i];
        const std::string& n = data.names[i];

        //Get data
        LinearMatrixType rotMatrix, scalMatrix;
        t.computeRotationScaling(&rotMatrix, &scalMatrix);
        Vector3<Scalar> traVec(t.translation());
        Rotation3<Scalar> rot(rotMatrix);
        Vector3<Scalar> scaVec(scalMatrix.diagonal());

        constexpr double eps = 1e-5;

        bool rotation = false;
        Vector3<Scalar> ang = eulerAnglesXYZFromRotation(rot);
        for (EigenId i = 0; i < ang.size(); ++i) {
            ang[i] = ang[i] / M_PI * 180.0;

            if (epsEqual(ang[i], 0.0, eps)) {
                ang[i] = 0.0;
            }
            else {
                rotation = true;
            }
        }

        for (EigenId i = 0; i < traVec.size(); ++i) {
            if (epsEqual(traVec[i], 0.0, eps)) {
                traVec[i] = 0.0;
            }
        }

        bool scaling = false;
        for (EigenId i = 0; i < scaVec.size(); ++i) {
            if (epsEqual(scaVec[i], 1.0, eps)) {
                scaVec[i] = 1.0;
            }
            else {
                scaling = true;
            }
        }

        fSkt << "j " << i << " " << n << " " << p;

        fSkt << " " << traVec.x() << " " << traVec.y() << " " << traVec.z();

        if (rotation || scaling) {
            fSkt << " " << ang.x() << " " << ang.y() << " " << ang.z();
        }

        if (scaling) {
            fSkt << " " << scaVec.x() << " " << scaVec.y() << " " << scaVec.z();
        }

        if (h) {
            fSkt << " " << "hidden";
        }

        fSkt << std::endl;
    }

    //Successfully loaded
    return true;
}

}
