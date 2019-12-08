#include "quad.h"

Quad::Quad() : ShapeBase()
{
//    std::vector<float> data({-1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
//                            -1.f, -1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f,
//                            1.f, -1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f,
//                            -1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
//                            1.f, -1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f,
//                            1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f});

    std::vector<float> data({-1.f, 0.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f,
                             -1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f,
                             1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f,
                             -1.f, 0.f, -1.f, 0.f, 1.f, 0.f, 0.f, 0.f,
                             1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f,
                             1.f, 0.f, -1.f, 0.f, 1.f, 0.f, 1.f, 0.f});
    buildVAO(data);
}
