#include "Model.h"

#include <map>

namespace Core
{

Model::Model()
{

}

Model::~Model()
{

}

void Model::Draw(Shader& shader)
{
    for(unsigned int meshIndex = 0; meshIndex < m_Meshes.size(); meshIndex++)
    {
        m_Meshes[meshIndex].Draw(shader);
    }
}

void Model::LoadTextures()
{
    for(unsigned int meshIndex = 0; meshIndex < m_Meshes.size(); meshIndex++)
    {
        for(unsigned int texIndex = 0; texIndex < m_Meshes[meshIndex].m_Material.m_Textures.size(); texIndex++)
        {
            Texture* texture = &m_Meshes[meshIndex].m_Material.m_Textures[texIndex];
            texture->m_RenderId = texture->LoadBMPCustom(texture->m_FilePath.c_str());
        }
    }
}

}