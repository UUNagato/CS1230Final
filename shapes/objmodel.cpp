#include "objmodel.h"
#include "gl/objloader.h"

ObjModel::ObjModel() : ShapeBase()
{

}

bool ObjModel::load(const char *path)
{
    std::vector<float> data;
    std::vector<unsigned int> indices;
    if (!ObjLoader::load(path, &data, &indices)) {
        return false;
    }

    buildVAO(data, indices.data(), indices.size());
    return true;
}
