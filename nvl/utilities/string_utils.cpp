/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "string_utils.h"

#include <regex>

namespace nvl {

/**
 * @brief Transform an input string in lower case.
 * @param string Input string
 * @return The lower case string
 */
NVL_INLINE std::string stringToLower(const std::string& string)
{
    std::string result = string;

    stringToLowerInPlace(result);

    return result;
}

/**
 * @brief Transform an input string in lower case.
 * @param string Input string
 */
NVL_INLINE void stringToLowerInPlace(std::string& string)
{
    std::transform(string.begin(), string.end(), string.begin(),
        [](unsigned char c)
    {
        return std::tolower(c);
    });
}

/**
 * @brief Transform an input string in upper case.
 * @param string Input string
 * @return The upper case string
 */
NVL_INLINE std::string stringToUpper(const std::string& string) {
    std::string result = string;

    stringToUpperInPlace(result);

    return result;
}

/**
 * @brief Transform an input string in upper case.
 * @param string Input string
*/
NVL_INLINE void stringToUpperInPlace(std::string& string) {
    std::transform(string.begin(), string.end(), string.begin(),
        [](unsigned char c)
    {
        return std::toupper(c);
    });
}

/**
 * @brief Split the string by a given separator
 * @param string Input string
 * @param separator Separator char
 * @return An array of strings
 */
NVL_INLINE std::vector<std::string> stringSplit(const std::string& string, char separator)
{
    std::vector<std::string> result;

    std::string currentString;
    for(const char& c : string) {
        if (c == separator) {
            result.push_back(currentString);
            currentString.clear();
        }
        else {
            currentString += c;
        }
    }
    result.push_back(currentString);

    return result;
}

/**
 * @brief Split the string by a given separator
 * @param string Input string
 * @param separator Separator char
 * @return An array of strings
 */
NVL_INLINE std::vector<std::string> stringSplit(const char* string, char separator)
{
    return stringSplit(std::string(string), separator);
}

/**
 * @brief Given an array of string, it joins the elements in a single string
 * @param stringArray Array of strings
 * @param start Start element of the array (default 0)
 * @param end End element of the array (default the end of the array)
 * @param separator Separator char (default space)
 * @param ignoreEmpty Do not join empty elements
 * @return Resulting string
 */
NVL_INLINE std::string stringUnion(
        const std::vector<std::string>& stringArray,
        const Index& start,
        const Index& end,
        const char separator,
        const bool ignoreEmpty)
{
    std::string result;

    Index realEnd = end;

    if (realEnd > stringArray.size())
        realEnd = stringArray.size();

    if (start < stringArray.size()) {
        result += stringArray[start];
        for (Index i = start + 1; i < realEnd; i++) {
            if (!ignoreEmpty || !stringArray[i].empty()) {
                result += separator;
                result += stringArray[i];
            }
        }
    }

    return result;
}

/**
 * @brief Given a stringstream, it joins the elements in a single string
 * @param stringStream String stream
 * @param separator Separator char (default space)
 * @param ignoreEmpty Do not join empty elements
 * @return Resulting string
 */
NVL_INLINE std::string stringUnion(
        std::istringstream& stringStream,
        const char separator,
        const bool ignoreEmpty)
{
    std::string result;

    bool first = true;
    while (!stringStream.eof()) {
        std::string tmpString;
        stringStream >> tmpString;

        if (!first)
            result += separator;
        else
            first = false;

        if (!ignoreEmpty || !tmpString.empty()) {
            result += tmpString;
        }
    }

    return result;
}

/**
 * @brief Trim a given string, erasing spaces at the beginning and in the end
 * @param s Input string
 * @return Trimmed string
 */
NVL_INLINE std::string stringTrim(const std::string& string) {
    std::string result = string;
    stringLeftTrimInPlace(result);
    return result;
}

/**
 * @brief Trim a given string, erasing spaces at the beginning and in the end
 * @param s Input string
 */
NVL_INLINE void stringTrimInPlace(std::string& string) {
    stringLeftTrimInPlace(string);
    stringRightTrimInPlace(string);
}

/**
 * @brief Left trim a given string, erasing spaces at the beginning
 * @param s Input string
 * @return Trimmed string
 */
NVL_INLINE std::string stringLeftTrim(const std::string& string)
{
    std::string result = string;

    stringLeftTrimInPlace(result);

    return result;
}

/**
 * @brief Left trim a given string, erasing spaces at the beginning
 * @param s Input string
 */
NVL_INLINE void stringLeftTrimInPlace(std::string& string)
{
    const std::string WHITESPACE = " \n\r\t\f\v";

    size_t start = string.find_first_not_of(WHITESPACE);
    if (start == std::string::npos) {
        string.erase(0, start);
    }
}

/**
 * @brief Right trim a given string, erasing spaces in the end
 * @param s Input string
 * @return Trimmed string
 */
NVL_INLINE std::string stringRightTrim(const std::string& string)
{
    std::string result = string;

    stringRightTrimInPlace(result);

    return result;
}

/**
 * @brief Right trim a given string, erasing spaces in the end
 * @param s Input string
 */
NVL_INLINE void stringRightTrimInPlace(std::string& string)
{
    const std::string WHITESPACE = " \n\r\t\f\v";

    size_t end = string.find_last_not_of(WHITESPACE);
    if (end == std::string::npos) {
        string.erase(end + 1, string.size());
    }
}

/**
 * @brief Check if the string contrains a numeric value
 * @param string Input string
 * @return True if string is numeric, false otherwise
 */
NVL_INLINE bool stringIsNumeric(std::string& string)
{
    return std::regex_match(string, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
}

/**
 * @brief Check if the string contrains a integer value
 * @param string Input string
 * @return True if string is integer, false otherwise
 */
NVL_INLINE bool stringIsInteger(std::string& string)
{
    return std::regex_match(string, std::regex("[+-]?[0-9][0-9]*"));
}

/**
 * @brief Check if the string contrains a float value
 * @param string Input string
 * @return True if string is numeric, false otherwise
 */
NVL_INLINE bool stringIsFloat(std::string& string)
{
    return std::regex_match(string, std::regex("[+-]?[0-9]+(\\.[0-9]+)?([Ee][+-]?[0-9]+)?"));
}

}
