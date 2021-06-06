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

    bool Import(const std::string& filePath, std::string& outName, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices);
    void ImportOBJ(std::ifstream& fileStream, std::string& outName, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices);


};

}