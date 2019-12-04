#include <iostream>
#include "shapebase.h"

#include "glew-1.10.0/include/GL/glew.h"

ShapeBase::ShapeBase() : m_vbo(0), m_vao(0), m_ebo(0)
{

}

ShapeBase::~ShapeBase()
{
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
    }

    if (m_ebo) {
        glDeleteBuffers(1, &m_ebo);
    }

    if (m_vbo) {
        glDeleteBuffers(1, &m_vbo);
    }
}

void ShapeBase::buildVAO(std::vector<float> &data, const unsigned int *indices, unsigned int vnumber)
{
    size_t total_size = data.size();
    if (indices) {
        m_numberOfVertex = vnumber;
    }
    else {
        m_numberOfVertex = total_size / 8;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_vbo) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_ebo) {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }

    // create new
    glGenBuffers(1, &m_vbo);
    if (!m_vbo) {
        std::cout << "Failed to generate vbo" << std::endl;
        return;
    }

    if (indices) {
        glGenBuffers(1, &m_ebo);
    }

    glGenVertexArrays(1, &m_vao);
    if (!m_vao) {
        std::cout << "Failed to generate vao" << std::endl;
        return;
    }
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, total_size * sizeof(float), data.data(), GL_STATIC_DRAW);
    if (m_ebo) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vnumber * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShapeBase::draw()
{
    if (m_vao) {
        glBindVertexArray(m_vao);
        if (m_ebo) {
            glDrawElements(GL_TRIANGLES, m_numberOfVertex, GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, m_numberOfVertex);
        }
    }
}
