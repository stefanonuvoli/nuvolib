/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_UTILITIES_FILE_UTILS_H
#define NVL_UTILITIES_FILE_UTILS_H

#include <nvl/nuvolib.h>

#include <string>

namespace nvl {

std::string filenameFile(const std::string& filename);
std::string filenameFile(const char* string);

std::string filenameName(const std::string& filename);
std::string filenameName(const char* string);

std::string filenamePath(const std::string& filename);
std::string filenamePath(const char* string);

std::string filenameExtension(const std::string& filename);
std::string filenameExtension(const char* string);

std::string filenameRelativePath(
        const std::string& path,
        const std::string& parentPath);

std::string filenameAbsolutePath(
        const std::string& path,
        const std::string& parentPath);

bool fileCopy(const std::string& source, const std::string& dest);
bool fileCopy(const char* source, const char* dest);

bool createDirectory(const std::string& dir);
bool createDirectory(const char* dir);

}

#include "file_utils.cpp"

#endif // NVL_UTILITIES_FILE_UTILS_H
