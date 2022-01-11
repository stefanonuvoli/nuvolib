#include "gl_textures.h"

#ifdef NVL_OPENGL_LOADED

#include <nvl/math/numeric_limits.h>

#ifdef NVL_QGLVIEWER_LOADED
#include <QGLViewer/qglviewer.h>
#include <QImage>
#else
#ifdef NVL_STB_LOADED
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/std_image.h>
#undef STB_IMAGE_STATIC
#undef STB_IMAGE_IMPLEMENTATION
#endif
#endif

namespace nvl {

NVL_INLINE int glLoadTextureImage(const std::string& filename, const GLint textureMode)
{
    int textureId = maxLimitValue<unsigned int>();

#ifdef NVL_QGLVIEWER_LOADED

    QImage img;
    bool imageLoaded = img.load(filename.c_str());
    if (imageLoaded) {
        QImage GL_formatted_image;
        GL_formatted_image = QGLWidget::convertToGLFormat(img);

        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        if (GL_formatted_image.hasAlphaChannel()) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GL_formatted_image.width(), GL_formatted_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, GL_formatted_image.bits());
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GL_formatted_image.width(), GL_formatted_image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, GL_formatted_image.bits());
        }

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);

        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        textureId = texture;

        GL_formatted_image = QImage();
        img = QImage();
    }

#else

#ifdef NVL_STB_LOADED
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(mat.diffuseMap().c_str(), &width, &height, &nrChannels, 0);

    if (data != nullptr) {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureMode);

        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        vTextures[mId] = texture;

        stbi_image_free(data);
    }

#endif

#endif

    return textureId;
}

}

#endif
