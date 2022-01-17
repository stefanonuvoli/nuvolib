/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "file_utils.h"

#include <fstream>

#include <filesystem>

namespace nvl {

/**
 * @brief Get the file name given the complete path
 * @param filename Filename
 * @return File name
 */
NVL_INLINE std::string filenameFile(const std::string& filename)
{
    Index lastSlashIndex = filename.find_last_of("/\\");

    return filename.substr(lastSlashIndex + 1, filename.size() - (lastSlashIndex + 1));
}

/**
 * @brief Get the file name given the complete path
 * @param filename Filename
 * @return File name
 */
NVL_INLINE std::string filenameFile(const char* string)
{
    return filenameFile(std::string(string));
}

/**
 * @brief Get the file name without extension, given the complete path
 * @param filename Filename
 * @return File name without extension
 */
NVL_INLINE std::string filenameName(const std::string& filename)
{
    Index lastDotIndex = filename.find_last_of(".");
    Index lastSlashIndex = filename.find_last_of("/\\");

    return filename.substr(lastSlashIndex + 1, lastDotIndex - (lastSlashIndex + 1));
}

/**
 * @brief Get the file name without extension, given the complete path
 * @param filename Filename
 * @return File name without extension
 */
NVL_INLINE std::string filenameName(const char* string)
{
    return filenameName(std::string(string));
}

/**
 * @brief Get the path of a file
 * @param filename Filename
 * @return Path
 */
NVL_INLINE std::string filenamePath(const std::string& filename)
{
    Index lastSlashIndex = filename.find_last_of("/\\");
    return filename.substr(0, lastSlashIndex + 1);
}

/**
 * @brief Get the path of the file
 * @param filename Filename
 * @return Path
 */
NVL_INLINE std::string filenamePath(const char* string)
{
    return filenamePath(std::string(string));
}

/**
 * @brief Get the extension of a file
 * @param filename Filename
 * @return File extension
 */
NVL_INLINE std::string filenameExtension(const std::string& filename)
{
    Index lastDotIndex = filename.find_last_of(".");
    return filename.substr(lastDotIndex + 1, filename.size() - (lastDotIndex + 1));;
}

/**
 * @brief Get the extension of a file
 * @param filename Filename
 * @return File extension
 */
NVL_INLINE std::string filenameExtension(const char* string)
{
    return filenameExtension(std::string(string));
}

/**
 * @brief Get the relative path of a file
 * @param path Path of the file
 * @param parentPath Path of the parent
 * @return Relative path of the file
 */
NVL_INLINE std::string filenameRelativePath(
        const std::string& path,
        const std::string& parentPath)
{
    std::string result = path;

    if (path.at(0) == '/') {
        if (path.find(parentPath, 0) == 0) {
            result.erase(0, parentPath.length());
        }
    }

    return result;
}

/**
 * @brief Get the absolute path of a file
 * @param path Path of the file
 * @param parentPath Path of the parent
 * @return Absolute path of the file
 */
NVL_INLINE std::string filenameAbsolutePath(
        const std::string& path,
        const std::string& parentPath)
{
    std::string result = path;

    if (path.at(0) != '/') {
        result = parentPath + path;
    }

    return result;
}

/**
 * @brief Copy a file
 * @param source Source file
 * @param dest Target file
 * @return True if file has been copied, false otherwise
 */
NVL_INLINE bool fileCopy(const std::string& source, const std::string& dest)
{
    std::ifstream ifs(source, std::ios::binary);
    if (!ifs.is_open()) {
        return false;
    }

    //Get pointer to associated buffer object
    std::filebuf* pbuf = ifs.rdbuf();

    //Get file size using buffer's members
    std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
    pbuf->pubseekpos(0, ifs.in);

    //Allocate memory to contain file data
    char* buffer = new char[size];

    //Get file data
    pbuf->sgetn(buffer,size);

    //Close resource
    ifs.close();

    std::ofstream ofs(dest, std::ios::binary);
    if (!ofs.is_open()) {
        return false;
    }

    //Write to output
    ofs.write(buffer, size);

    //Close resource
    ofs.close();

    delete[] buffer;

    return true;
}

/**
 * @brief Copy a file
 * @param source Source file
 * @param dest Target file
 * @return True if file has been copied, false otherwise
 */
NVL_INLINE bool fileCopy(const char* source, const char* dest)
{
    return fileCopy(std::string(source), std::string(dest));
}

/**
 * @brief Create a directory
 * @param dir Directory
 * @return True if the directory has been created, false otherwise
 */
NVL_INLINE bool createDirectory(const std::string& dir)
{
    return std::filesystem::create_directories(dir);
}

/**
 * @brief Create a directory
 * @param dir Directory
 * @return True if the directory has been created, false otherwise
 */
NVL_INLINE bool createDirectory(const char* dir)
{
    return createDirectory(std::string(dir));
}

}
