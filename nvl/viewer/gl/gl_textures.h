#ifndef NVL_GL_TEXTURES_H
#define NVL_GL_TEXTURES_H

#include <nvl/nuvolib.h>

#include <string>

namespace nvl {

int glLoadTextureImage(const std::string& filename);

}

#include "gl_textures.cpp"

#endif // NVL_GL_TEXTURES_H
