#include "objectLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

mesh objectLoader::loadOBJ(const std::string& filepath)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;

    std::vector<float> finalVertices;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ: " << filepath << std::endl;
        return mesh(std::vector<float>());
    }
    std::cerr << "Successfully opened OBJ: " << filepath << std::endl;
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            positions.push_back(v);
        }
        else if (type == "vt") {
            glm::vec2 t;
            ss >> t.x >> t.y;
            texcoords.push_back(t);
        }
        else if (type == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (type == "f")
        {
            for (int i = 0; i < 3; i++)
            {
                std::string vert;
                ss >> vert;

                int p = 0, t = 0, n = 0;
                // detect format
                if (vert.find('/') == std::string::npos)
                {
                    p = std::stoi(vert);
                }
                else if (vert.find("//") != std::string::npos)
                {
                    sscanf_s(vert.c_str(), "%d//%d", &p, &n);
                }
                else
                {
                    sscanf_s(vert.c_str(), "%d/%d/%d", &p, &t, &n);
                }

                glm::vec3 pos = positions[p - 1];

                glm::vec2 tex(0.0f);  
                if (t > 0) tex = texcoords[t - 1];

                glm::vec3 color(1.0f); 

                finalVertices.insert(finalVertices.end(), 
                    {
                    pos.x, pos.y, pos.z,
                    tex.x, tex.y,
                    color.x, color.y, color.z
                    });
            }

        }
    }

    return mesh(finalVertices);

}
