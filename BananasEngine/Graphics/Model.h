#pragma once

#include "../Core/Common.h"
#include "Mesh.h"
#include "Shader.h"

namespace GraphicsEngine
{

class Model
{
public:
    std::vector<Mesh> m_Meshes;

public:
    Model();
    ~Model();

    void Draw(Shader& shader);
    void LoadModel(const char* filePath);
    void LoadTextures();
};

}