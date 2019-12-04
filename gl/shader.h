#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>
#include <glm/glm.hpp>

// Represent shader

class Shader
{
public:
    Shader();
    Shader(const char *vertexFile, const char *fragFile);

    virtual ~Shader();

    bool compile(const char *vertexFile, const char *fragFile);

    void setUniform(const std::string &name, float f);
    void setUniform(const std::string &name, const glm::vec2 &vec2);
    void setUniform(const std::string &name, const glm::vec3 &vec3);
    void setUniform(const std::string &name, const glm::vec4 &vec4);
    void setUniform(const std::string &name, int i);
    void setUniform(const std::string &name, const glm::ivec2 &ivec2);
    void setUniform(const std::string &name, const glm::ivec3 &ivec3);
    void setUniform(const std::string &name, const glm::ivec4 &ivec4);
    void setUniform(const std::string &name, bool b);
    void setUniform(const std::string &name, const glm::bvec2 &bvec2);
    void setUniform(const std::string &name, const glm::bvec3 &bvec3);
    void setUniform(const std::string &name, const glm::bvec4 &bvec4);
    void setUniform(const std::string &name, const glm::mat2 &mat2);
    void setUniform(const std::string &name, const glm::mat3 &mat3);
    void setUniform(const std::string &name, const glm::mat4 &mat4);

    void bind();
    void unbind();
    unsigned int getID() const { return m_handle; }

private:
    unsigned int m_handle;

    std::string readShaderFile(const char *file);
};

#endif // SHADER_H
