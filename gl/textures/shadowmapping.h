#ifndef SHADOWMAPPING_H
#define SHADOWMAPPING_H

#include <memory>
#include <gl/shader.h>
#include <glm.hpp>

class ShadowMapping
{
public:
    ShadowMapping();
    ~ShadowMapping();

    void setup();
    void finish();

    void setLightPos(const glm::vec3 &pos) {light_pos = pos;}
    void setLightDir(const glm::vec3 &dir) {light_dir = dir;}

    void bindShadowMapping();
    void unbindShadowMapping();

    static const int SHADOW_MAPPING_WIDTH = 1024;
    static const int SHADOW_MAPPING_HEIGHT = 1024;

private:
    unsigned int m_fbo;
    unsigned int m_texture;

    glm::vec3 light_pos;
    glm::vec3 light_dir;

    std::unique_ptr<Shader> m_smShader;
};

#endif // SHADOWMAPPING_H
