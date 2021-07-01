#ifndef GRAPHICS_ENGINE_IMPORTER_H
#define GRAPHICS_ENGINE_IMPORTER_H

#include "../Core/Common.h"
#include "Mesh.h"

#include <map>

namespace GraphicsEngine
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

#endif