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
    Model();
    ~Model();

    void Draw(Shader& shader);
    void LoadTextures();
};

}