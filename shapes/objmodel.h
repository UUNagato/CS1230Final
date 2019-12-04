#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "shapes/shapebase.h"

class ObjModel : public ShapeBase
{
public:
    ObjModel();

    bool load(const char *path);
};

#endif // OBJMODEL_H
