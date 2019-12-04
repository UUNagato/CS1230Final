#ifndef SHAPEBASE_H
#define SHAPEBASE_H

#include <vector>

// base class for all shape.
// in order to simplify process, all shapes must be triangle mesh and provide data by ordering:
// pos, normal, uv

class ShapeBase
{
public:
    ShapeBase();
    virtual ~ShapeBase();

    virtual void draw();

protected:
    void buildVAO(std::vector<float> &data, const unsigned int *indices = nullptr, unsigned int vnumber = 0);

private:
    unsigned int m_vbo;
    unsigned int m_vao;
    unsigned int m_ebo;
    unsigned int m_numberOfVertex;
};

#endif // SHAPEBASE_H
