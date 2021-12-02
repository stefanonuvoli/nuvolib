#include "string_utils.h"

namespace nvl {

std::string stringToLower(const std::string& string) {
    std::string result = string;

    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c)
    {
        return std::tolower(c);
    });

    return result;
}

std::string stringToUpper(const std::string& string) {
    std::string result = string;

    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c)
    {
        return std::toupper(c);
    });

    return result;
}

std::vector<std::string> stringSplit(const std::string& string, char separator)
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

std::vector<std::string> stringSplit(const char* string, char separator)
{
    return stringSplit(std::string(string), separator);
}

}
