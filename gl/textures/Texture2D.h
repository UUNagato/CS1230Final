#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include "Texture.h"

#include "GL/glew.h"

class Texture2D : public Texture {
public:
    Texture2D();
    Texture2D(unsigned char *data, int width, int height, GLenum type = GL_UNSIGNED_BYTE);

    bool open(std::string filepath);

    virtual void bind() const override;
    virtual void unbind() const override;
};

#endif // TEXTURE2D_H
