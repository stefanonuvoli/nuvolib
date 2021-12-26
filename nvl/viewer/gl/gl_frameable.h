#ifndef NVL_VIEWER_GL_FRAMEABLE_H
#define NVL_VIEWER_GL_FRAMEABLE_H

#include <nvl/nuvolib.h>

#ifdef NVL_OPENGL_LOADED

#include <nvl/math/point.h>

#include <nvl/viewer/gl/opengl_headers.h>
#include <nvl/viewer/interfaces/frameable.h>

namespace nvl {

class GLFrameable : public Frameable
{

public:

    GLFrameable();
    virtual ~GLFrameable() = default;

    void loadFrame() const;
    void unloadFrame() const;

};

}

#endif

#include "gl_frameable.cpp"

#endif // NVL_VIEWER_GL_FRAMEABLE_H
