#include "Model.h"

#include "Importer.h"

namespace GraphicsEngine
{

Model::Model()
{

}

Model::~Model()
{

}

void Model::Draw(Shader& shader)
{
    for(uint32 meshIndex = 0; meshIndex < m_meshes.size(); meshIndex++)
    {
        m_meshes[meshIndex].Draw(shader);
    }
}

void Model::LoadModel(const std::string& filePath)
{
    std::vector<std::string> outNames;
    std::vector<std::vector<Vertex>> outVertices;
    std::vector<std::vector<uint32>> outIndices;
    // TODO(neil): outMeshSizes is only used to loop through an array in Importer. 
    //             Pass back a meshCount instead. Use the meshSizes buffer for internal function.
    std::vector<uint32> outMeshSizes;
    std::vector<Material> outMaterials;
    
    Importer import;
    import.LoadModel(filePath, outNames, outVertices, outIndices, outMeshSizes, outMaterials);

    // TODO(neil): Optimize to use same material for each mesh instead 
    // of creating new ones per mesh if they are the same material.
    for(uint32 i = 0; i < outMeshSizes.size(); i++)
    {
        Mesh mesh{outNames[i], outVertices[i], outIndices[i], outMaterials[i]};
        m_meshes.push_back(mesh);
    }
}

void Model::LoadMeshes()
{
    for(uint32 meshIndex = 0; meshIndex < m_meshes.size(); meshIndex++)
    {
        m_meshes[meshIndex].SetupMesh();
    }
}

void Model::LoadTextures()
{
    for(uint32 meshIndex = 0; meshIndex < m_meshes.size(); meshIndex++)
    {
        for(uint32 texIndex = 0; texIndex < m_meshes[meshIndex].m_material.m_textures.size(); texIndex++)
        {
            Texture* texture = &m_meshes[meshIndex].m_material.m_textures[texIndex];
            texture->m_renderId = texture->LoadBMPCustom(texture->m_filePath.c_str());
        }
    }
}

}