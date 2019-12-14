#include "shadowmapping.h"
//#include <glew-1.10.0\include\GL\glew.h>
#include "GL/glew.h"

ShadowMapping::ShadowMapping() : m_fbo(0), m_texture(0), m_width(SHADOW_MAPPING_WIDTH),
    m_height(SHADOW_MAPPING_HEIGHT)
{

}

ShadowMapping::~ShadowMapping()
{
    finish();
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
    }

    if (m_fbo) {
        glDeleteFramebuffers(1, &m_fbo);
    }
}

void ShadowMapping::setup()
{
    if (!m_fbo)
        glGenFramebuffers(1, &m_fbo);
    if (!m_fbo)
        return;

    if (!m_texture) {
        glGenTextures(1, &m_texture);
        if (!m_texture)
            return;
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture, 0);
}

void ShadowMapping::finish()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapping::bindShadowMapping()
{
    if (m_texture)
        glBindTexture(GL_TEXTURE_2D, m_texture);
}

void ShadowMapping::unbindShadowMapping()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowMapping::setSize(int width, int height)
{
    m_width = width > 0 ? width : 1;
    m_height = height > 0 ? height : 1;

    if (m_texture)
        glDeleteTextures(1, &m_texture);

    glGenTextures(1, &m_texture);
    if (!m_texture)
        return;

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}
