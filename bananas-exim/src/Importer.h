#pragma once

#include "Common.h"
#include "Mesh.h"

#include <map>

namespace Core
{

class Importer
{

public:
    Importer();
    ~Importer();

    bool LoadModel(const std::string& filePath, std::vector<std::string>& outNames, std::vector<std::vector<Vertex>>& outVertices, std::vector<std::vector<unsigned int>>& outIndices, std::vector<unsigned int>& outMeshSizes, std::vector<Material>& outMaterials);

private:
    void LoadModelOBJ(std::ifstream& fileStream, std::vector<std::string>& outNames, std::vector<std::vector<Vertex>>& outVertices, std::vector<std::vector<unsigned int>>& outIndices, std::vector<unsigned int>& outMeshSizes, std::string& path, std::vector<Material>& outMaterial);
    bool LoadModelMTL(std::string& filePath, std::vector<std::string>& usemtl, std::vector<Material>& outMaterials);
};

}
