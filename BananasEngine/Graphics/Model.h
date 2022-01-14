#ifndef GRAPHICS_ENGINE_MODEL_H
#define GRAPHICS_ENGINE_MODEL_H

#include "../Core/Common.h"
#include "Mesh.h"
#include "Shader.h"

namespace GraphicsEngine
{

class Model
{
public:
    std::string m_name;
    std::vector<Mesh> m_meshes;

public:
    Model();
    ~Model();

    void Draw(Shader& shader);
    void LoadModel(const std::string& filePath);
    void LoadMeshes();
    void LoadTextures();
};

}

#endif
