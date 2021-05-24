#pragma once

#include "Common.h"
#include "Mesh.h"
#include "Shader.h"

namespace Core
{

class Model
{
public:
    std::vector<Mesh> m_Meshes;

public:
    Model(const std::vector<Mesh> meshes);
    ~Model();

    void Draw(Shader& shader);

    bool Import(const char* filePath);

    std::string ImportOBJ(const std::string& content);

};

}