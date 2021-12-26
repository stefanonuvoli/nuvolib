#ifndef NVL_VIEWER_OPENGL_HEADERS_H
#define NVL_VIEWER_OPENGL_HEADERS_H

#ifdef NVL_OPENGL_LOADED

#ifdef __APPLE__
#include <OpenGL/glew.h>
#include <OpenGL/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#endif

#endif // NVL_VIEWER_OPENGL_HEADERS_H
