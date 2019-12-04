#include <iostream>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "shader.h"

#include "glew-1.10.0/include/GL/glew.h"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader() : m_handle(0)
{
    m_handle = glCreateProgram();
}

Shader::Shader(const char *vertexFile, const char *fragFile) : m_handle(0)
{
    Shader();
    compile(vertexFile, fragFile);
}

Shader::~Shader()
{
    if (m_handle) {
        glDeleteProgram(m_handle);
    }
}



bool Shader::compile(const char *vertexFile, const char *fragFile)
{
    if (!m_handle) {
        m_handle = glCreateProgram();
    }

    if (!m_handle) {
        std::cout << "Cannot create shader program" << std::endl;
        return false;
    }

    std::string vertex_src = readShaderFile(vertexFile);
    std::string frag_src = readShaderFile(fragFile);

    if (vertex_src.empty() || frag_src.empty())
        return false;

    unsigned int vertexShader, fragShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *cstr = vertex_src.c_str();
    glShaderSource(vertexShader, 1, &cstr, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEXSHADERCOMPILE::" << infoLog << std::endl;
        return false;
    }

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    cstr = frag_src.c_str();
    glShaderSource(fragShader, 1, &cstr, NULL);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGSHADERCOMPILE::" << infoLog << std::endl;
        return false;
    }

    if (m_handle) {
        glAttachShader(m_handle, vertexShader);
        glAttachShader(m_handle, fragShader);
        glLinkProgram(m_handle);


        glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_handle, 512, NULL, infoLog);
            std::cout << "ERROR::SHADERLINK::" << infoLog << std::endl;
            return false;
        }

        return true;
    }

    return false;
}

std::string Shader::readShaderFile(const char *file)
{
    std::string path_file(file);
    QString vertFilePath = QString::fromStdString(path_file);
    QFile vertFile(vertFilePath);
    if (vertFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream vertStream(&vertFile);
        QString contents = vertStream.readAll();
        return contents.toStdString();
    }
    std::cout << "Cannot open file:" << path_file;
    return "";
}

void Shader::setUniform(const std::string &name, float f) {
    glUniform1f(glGetUniformLocation(m_handle, name.c_str()), f);
}

void Shader::setUniform(const std::string &name, const glm::vec2 &vec2) {
    glUniform2fv(glGetUniformLocation(m_handle, name.c_str()), 1, glm::value_ptr(vec2));
}

void Shader::setUniform(const std::string &name, const glm::vec3 &vec3) {
    glUniform3fv(glGetUniformLocation(m_handle, name.c_str()), 1, glm::value_ptr(vec3));
}

void Shader::setUniform(const std::string &name, const glm::vec4 &vec4) {
    glUniform4fv(glGetUniformLocation(m_handle, name.c_str()), 1, glm::value_ptr(vec4));
}

void Shader::setUniform(const std::string &name, int i) {
    glUniform1i(glGetUniformLocation(m_handle, name.c_str()), i);
}

void Shader::setUniform(const std::string &name, const glm::ivec2 &ivec2) {
    glUniform2iv(glGetUniformLocation(m_handle, name.c_str()), 1, glm::value_ptr(ivec2));
}

void Shader::setUniform(const std::string &name, const glm::ivec3 &ivec3) {
    glUniform3iv(glGetUniformLocation(m_handle, name.c_str()), 1, glm::value_ptr(ivec3));
}

void Shader::setUniform(const std::string &name, const glm::ivec4 &ivec4) {
    glUniform4iv(glGetUniformLocation(m_handle, name.c_str()), 1, glm::value_ptr(ivec4));
}

void Shader::setUniform(const std::string &name, bool b) {
    glUniform1i(glGetUniformLocation(m_handle, name.c_str()), static_cast<GLint>(b));
}

void Shader::setUniform(const std::string &name, const glm::bvec2 &bvec2) {
    glUniform2iv(glGetUniformLocation(m_handle, name.c_str()), 1, glm::value_ptr(glm::ivec2(bvec2)));
}

void Shader::setUniform(const std::string &name, const glm::bvec3 &bvec3) {
    glUniform3iv(glGetUniformLocation(m_handle, name.c_str()), 1, glm::value_ptr(glm::ivec3(bvec3)));
}

void Shader::setUniform(const std::string &name, const glm::bvec4 &bvec4) {
    glUniform4iv(glGetUniformLocation(m_handle, name.c_str()), 1, glm::value_ptr(glm::ivec4(bvec4)));
}

void Shader::setUniform(const std::string &name, const glm::mat2 &mat2) {
    glUniformMatrix2fv(glGetUniformLocation(m_handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat2));
}

void Shader::setUniform(const std::string &name, const glm::mat3 &mat3) {
    glUniformMatrix3fv(glGetUniformLocation(m_handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat3));
}

void Shader::setUniform(const std::string &name, const glm::mat4 &mat4) {
    glUniformMatrix4fv(glGetUniformLocation(m_handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::bind()
{
    if (m_handle) {
        glUseProgram(m_handle);
    }
}

void Shader::unbind()
{
    glUseProgram(0);
}
