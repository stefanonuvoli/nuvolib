#include "file_utils.h"

#include <fstream>

#include <filesystem>

namespace nvl {

NVL_INLINE std::string filenameFile(const std::string& filename)
{
    Index lastSlashIndex = filename.find_last_of("/\\");

    return filename.substr(lastSlashIndex + 1, filename.size() - (lastSlashIndex + 1));
}

NVL_INLINE std::string filenameFile(const char* string)
{
    return filenameFile(std::string(string));
}

NVL_INLINE std::string filenameName(const std::string& filename)
{
    Index lastDotIndex = filename.find_last_of(".");
    Index lastSlashIndex = filename.find_last_of("/\\");

    return filename.substr(lastSlashIndex + 1, lastDotIndex - (lastSlashIndex + 1));
}

NVL_INLINE std::string filenameName(const char* string)
{
    return filenameName(std::string(string));
}

NVL_INLINE std::string filenamePath(const std::string& filename)
{
    Index lastSlashIndex = filename.find_last_of("/\\");
    return filename.substr(0, lastSlashIndex + 1);
}

NVL_INLINE std::string filenamePath(const char* string)
{
    return filenamePath(std::string(string));
}

NVL_INLINE std::string filenameExtension(const std::string& filename)
{
    Index lastDotIndex = filename.find_last_of(".");
    return filename.substr(lastDotIndex + 1, filename.size() - (lastDotIndex + 1));;
}

NVL_INLINE std::string filenameExtension(const char* string)
{
    return filenameExtension(std::string(string));
}

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

NVL_INLINE bool fileCopy(const std::string& source, const std::string& dest)
{
    std::ifstream sourceStream(source, std::ios::binary);
    if (!sourceStream.is_open()) {
        return false;
    }
    std::ofstream destStream(dest, std::ios::binary);
    if (!destStream.is_open()) {
        return false;
    }

    destStream << sourceStream.rdbuf();

    sourceStream.close();
    destStream.close();

    return true;
}

NVL_INLINE bool fileCopy(const char* source, const char* dest)
{
    return fileCopy(std::string(source), std::string(dest));
}

NVL_INLINE bool createDirectory(const std::string& dir)
{
    return std::filesystem::create_directories(dir);
}

NVL_INLINE bool createDirectory(const char* dir)
{
    return createDirectory(std::string(dir));
}

}
