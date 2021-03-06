#include "Texture.h"

#include <cassert>
#include <utility>

#include <GL/glew.h>

Texture::Texture() :
    m_handle(0)
{
    glGenTextures(1, &m_handle);
}

Texture::Texture(Texture &&that) :
    m_handle(that.m_handle)
{
    that.m_handle = 0;
}

Texture& Texture::operator=(Texture &&that) {
    this->~Texture();
    m_handle = that.m_handle;
    that.m_handle = 0;
    return *this;
}

Texture::~Texture()
{
    // TODO Don't forget to delete!
    glDeleteTextures(1, &m_handle);
    m_handle = 0;
}

unsigned int Texture::id() const {
    return m_handle;
}
