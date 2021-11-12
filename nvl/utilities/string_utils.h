#ifndef NVL_UTITLIES_STRING_UTILS_H
#define NVL_UTITLIES_STRING_UTILS_H

#include <nvl/nuvolib.h>

#include <vector>
#include <string>

namespace nvl {

std::vector<std::string> splitString(const std::string& string, char separator = ' ');
std::vector<std::string> splitString(const char* string, char separator = ' ');

}

#endif // NVL_UTITLIES_STRING_UTILS_H
