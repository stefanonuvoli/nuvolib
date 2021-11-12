#include "file_utils.h"

#include <fstream>

namespace nvl {

NVL_INLINE std::string filenameFile(const std::string& filename)
{
    Index lastSlashIndex = filename.find_last_of("/");

    return filename.substr(lastSlashIndex + 1, filename.size() - (lastSlashIndex + 1));
}

NVL_INLINE std::string filenameFile(const char* string)
{
    return filenameFile(std::string(string));
}

NVL_INLINE std::string filenameName(const std::string& filename)
{
    Index lastDotIndex = filename.find_last_of(".");
    Index lastSlashIndex = filename.find_last_of("/");

    return filename.substr(lastSlashIndex + 1, lastDotIndex - (lastSlashIndex + 1));
}

NVL_INLINE std::string filenameName(const char* string)
{
    return filenameName(std::string(string));
}

NVL_INLINE std::string filenamePath(const std::string& filename)
{
    Index lastSlashIndex = filename.find_last_of("/");
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

NVL_INLINE void fileCopy(const std::string& source, const std::string& dest)
{
    std::ifstream sourceStream(source, std::ios::binary);
    std::ofstream destStream(dest, std::ios::binary);

    destStream << sourceStream.rdbuf();

    sourceStream.close();
    destStream.close();
}

NVL_INLINE void fileCopy(const char* source, const char* dest)
{
    return fileCopy(std::string(source), std::string(dest));
}

}
