// ======================================================================
// Wavefront .obj loader
// ======================================================================
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class ObjLoader
{
public:
    static bool load(const std::string file_name,
                     std::vector<float> *data,
                     std::vector<unsigned int> *indices);

private:
    static void split(const std::string &s, std::vector<std::string> &tokens, const char *delim);
};

#endif // OBJLOADER_H
