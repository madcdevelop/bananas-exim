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

    bool Import(const std::string& filePath, std::vector<std::string>& outNames, std::vector<std::vector<Vertex>>& outVertices, std::vector<std::vector<unsigned int>>& outIndices, std::vector<unsigned int>& outMeshSizes);
    void ImportOBJ(std::ifstream& fileStream, std::vector<std::string>& outNames, std::vector<std::vector<Vertex>>& outVertices, std::vector<std::vector<unsigned int>>& outIndices, std::vector<unsigned int>& outMeshSizes);


};

}