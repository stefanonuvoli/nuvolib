/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_UTITLIES_STRING_UTILS_H
#define NVL_UTITLIES_STRING_UTILS_H

#include <nvl/nuvolib.h>

#include <vector>
#include <string>
#include <fstream>

#include <nvl/math/numeric_limits.h>

namespace nvl {

std::string stringToLower(const std::string& string);
void stringToLowerInPlace(std::string& string);

std::string stringToUpper(const std::string& string);
void stringToUpperInPlace(std::string& string);

std::vector<std::string> stringSplit(const std::string& string, char separator = ' ');
std::vector<std::string> stringSplit(const char* string, char separator = ' ');

std::string stringUnion(
        const std::vector<std::string>& stringArray,
        const Index& start = 0,
        const Index& end = nvl::maxLimitValue<Index>(),
        const char separator = ' ',
        const bool ignoreEmpty = false);

std::string stringUnion(
        std::istringstream& stringStream,
        const char separator = ' ',
        const bool ignoreEmpty = false);

std::string stringTrim(const std::string& string);
void stringTrimInPlace(std::string& string);

std::string stringLeftTrim(const std::string& string);
void stringLeftTrimInPlace(std::string& string);

std::string stringRightTrim(const std::string& string);
void stringRightTrimInPlace(std::string& string);

bool stringIsNumeric(std::string& string);
bool stringIsInteger(std::string& string);
bool stringIsFloat(std::string& string);

}

#include "string_utils.cpp"

#endif // NVL_UTITLIES_STRING_UTILS_H
