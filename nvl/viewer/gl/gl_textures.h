#ifndef NVL_GL_TEXTURES_H
#define NVL_GL_TEXTURES_H

#include <nvl/nuvolib.h>

#ifdef NVL_OPENGL_LOADED

#include <nvl/viewer/gl/opengl_headers.h>

#include <string>

namespace nvl {

int glLoadTextureImage(const std::string& filename, const GLint textureMode = GL_MODULATE);

}

#include "gl_textures.cpp"

#endif

#endif // NVL_GL_TEXTURES_H
