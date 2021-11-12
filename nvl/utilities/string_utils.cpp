#include "string_utils.h"

namespace nvl {

std::vector<std::string> splitString(const std::string& string, char separator)
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

std::vector<std::string> splitString(const char* string, char separator)
{
    return splitString(std::string(string), separator);
}

}
