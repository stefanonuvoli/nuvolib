#ifndef NVL_UTITLIES_STRING_UTILS_H
#define NVL_UTITLIES_STRING_UTILS_H

#include <nvl/nuvolib.h>

#include <vector>
#include <string>

namespace nvl {

std::string stringToLower(const std::string& string);
std::string stringToUpper(const std::string& string);

std::vector<std::string> stringSplit(const std::string& string, char separator = ' ');
std::vector<std::string> stringSplit(const char* string, char separator = ' ');

}

#endif // NVL_UTITLIES_STRING_UTILS_H
