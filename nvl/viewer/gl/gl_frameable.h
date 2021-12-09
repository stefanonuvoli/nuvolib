#ifndef NVL_VIEWER_GL_FRAMEABLE_H
#define NVL_VIEWER_GL_FRAMEABLE_H

#include <nvl/nuvolib.h>

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

#include "gl_frameable.cpp"

#endif // NVL_VIEWER_GL_FRAMEABLE_H
