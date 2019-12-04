#include <map>
#include <iostream>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include "objloader.h"

// The load only support triangle mesh.
//bool ObjLoader::load(const std::string file_name,
//                     std::vector<float> *data,
//                     std::vector<unsigned int> *indices)
//{
//    // no data container, loading is meaningless.
//    if (!data) {
//        return false;
//    }

//    std::vector<glm::vec3> pos, norm;
//    std::vector<glm::vec2> uvs;
//    std::map<std::string, size_t> index_map;
//    enum { UNDEFINED, POS, POS_UV, POS_UV_NORM, POS_NORM }
//         mode = UNDEFINED;

//    std::ifstream in(file_name);

//    // clean up
//    data->clear();
//    indices->clear();

//    if (in.is_open()) {
//        try {
//        std::string token;
//        while (in >> token) {
//            if (token == std::string("v")) {
//                // position, try to load 3 floats
//                glm::vec3 p;
//                in >> p.x >> p.y >> p.z;
//                pos.push_back(p);
//            }
//            else if (token == std::string("vn")) {
//                // normal, try to load 3 floats
//                glm::vec3 n;
//                in >> n.x >> n.y >> n.z;
//                norm.push_back(n);
//            }
//            else if (token == std::string("vt")) {
//                // UV, try to load 2 floats
//                glm::vec2 uv;
//                in >> uv.x >> uv.y;
//                uvs.push_back(uv);
//            }
//            else if (token == std::string("f")) {
//                // read index
//                std::string rawText;
//                size_t num = 0;
//                for (num = 0; num < 3; ++num) {
//                    in >> rawText;
//                    if (indices && index_map.find(rawText) != index_map.end()) {
//                        indices->push_back(index_map[rawText]);
//                        continue;
//                    }

//                    if (rawText.find("//") != std::string::npos) {
//                        // not supported yet
//                        return false;
//                    }
//                    else if (rawText.find("/") != std::string::npos) {
//                        // only support pos, uv, norm
//                        std::vector<std::string> tokens;
//                        split(rawText, tokens, "/");
//                        if (tokens.size() != 3)
//                            return false;
//                        unsigned int p_i, uv_i, n_i;
//                        p_i = std::stoi(tokens[0]) - 1;
//                        uv_i = std::stoi(tokens[1]) - 1;
//                        n_i = std::stoi(tokens[2]) - 1;

//                        // add those value
//                        data->push_back(pos[p_i].x); data->push_back(pos[p_i].y);
//                        data->push_back(pos[p_i].z);

//                        data->push_back(norm[n_i].x); data->push_back(norm[n_i].y);
//                        data->push_back(norm[n_i].z);

//                        data->push_back(uvs[uv_i].x); data->push_back(uvs[uv_i].y);

//                        // add index
//                        if (indices) {
//                            indices->push_back(indices->size());
//                        }
//                    }
//                }
//            }
//        }

//        return true;
//        }
//        catch (...) {
//            return false;
//        }
//    }

//    // cannot open or doesn't exist
//    return false;
//}

bool ObjLoader::load(const std::string file_name,
                     std::vector<float> *data,
                     std::vector<unsigned int> *indices)
{
    // no data container, loading is meaningless.
    if (!data) {
        return false;
    }

    std::vector<glm::vec3> pos, norm;
    std::vector<glm::vec2> uvs;
    std::map<std::string, size_t> index_map;
    enum { UNDEFINED, POS, POS_UV, POS_UV_NORM, POS_NORM }
         mode = UNDEFINED;

    QFile objFile(QString::fromStdString(file_name));
    if (!objFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
        std::cout << "Failed to open obj file:" << file_name << std::endl;
        return false;
    }
    QTextStream in(&objFile);

    // clean up
    data->clear();
    indices->clear();

    try {
        QString token;
        while (!in.atEnd()) {
            in >> token;
            if (token == "v") {
                // position, try to load 3 floats
                glm::vec3 p;
                in >> p.x >> p.y >> p.z;
                pos.push_back(p);
            }
            else if (token == "vn") {
                // normal, try to load 3 floats
                glm::vec3 n;
                in >> n.x >> n.y >> n.z;
                norm.push_back(n);
            }
            else if (token == "vt") {
                // UV, try to load 2 floats
                glm::vec2 uv;
                in >> uv.x >> uv.y;
                uvs.push_back(uv);
            }
            else if (token == "f") {
                // read index
                QString buffer;
                std::string rawText;
                size_t num = 0;
                for (num = 0; num < 3; ++num) {
                    in >> buffer;
                    rawText = buffer.toStdString();
                    if (indices && index_map.find(rawText) != index_map.end()) {
                        indices->push_back(index_map[rawText]);
                        continue;
                    }

                    if (rawText.find("//") != std::string::npos) {
                        // not supported yet
                        return false;
                    }
                    else if (rawText.find("/") != std::string::npos) {
                        // only support pos, uv, norm
                        std::vector<std::string> tokens;
                        split(rawText, tokens, "/");
                        if (tokens.size() != 3)
                            return false;
                        unsigned int p_i, uv_i, n_i;
                        p_i = std::stoi(tokens[0]) - 1;
                        uv_i = std::stoi(tokens[1]) - 1;
                        n_i = std::stoi(tokens[2]) - 1;

                        // add those value
                        data->push_back(pos[p_i].x); data->push_back(pos[p_i].y);
                        data->push_back(pos[p_i].z);

                        data->push_back(norm[n_i].x); data->push_back(norm[n_i].y);
                        data->push_back(norm[n_i].z);

                        data->push_back(uvs[uv_i].x); data->push_back(1.f - uvs[uv_i].y);

                        // add index
                        if (indices) {
                            indices->push_back(indices->size());
                        }
                    }
                }
            }
        }

        return true;
    }
    catch (...) {
        return false;
    }

    // cannot open or doesn't exist
    return false;
}

void ObjLoader::split(const std::string &s, std::vector<std::string> &tokens, const char *delim)
{
    std::string::size_type lastPos = s.find_first_not_of(delim, 0);
    std::string::size_type pos = s.find_first_of(delim, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delim, pos);
        pos = s.find_first_of(delim, lastPos);
    }
}
