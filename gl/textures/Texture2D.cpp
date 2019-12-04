#include "Texture2D.h"

#include <QImage>
#include <QString>
#include <utility>

Texture2D::Texture2D() : Texture ()
{
}

Texture2D::Texture2D(unsigned char *data, int width, int height, GLenum type) : Texture()
{
    GLenum internalFormat = type == GL_FLOAT ? GL_RGBA32F : GL_RGBA;

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, type, data);
    unbind();
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture2D::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture2D::open(std::string filepath)
{
    QImage image;
    QImage tmp;
    unsigned char *data = nullptr;
    if(image.load(QString::fromStdString(filepath))) {
        if (image.format() != QImage::Format_RGBX8888) {
            tmp = image.convertToFormat(QImage::Format::Format_RGBX8888);
            data = tmp.bits();
        } else {
            data = image.bits();
        }

        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        unbind();

        return true;
    }
    return false;
}
