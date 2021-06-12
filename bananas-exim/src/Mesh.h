#pragma once

#include "Common.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

#include "Shader.h"

namespace Core
{

class Mesh 
{
public:
    std::string m_Name;
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<Texture> m_Textures;

private:
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_IBO;

public:
    Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Texture>& tex);
    ~Mesh();

    void Draw(Shader& shader);

private:
    void SetupMesh();
};

}
