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

    VertexBuffer m_VertexBuffer;
    IndexBuffer  m_IndexBuffer;
    std::vector<Texture> m_Textures;
    unsigned int m_VAO;


public:
    Mesh(const std::string& name, const VertexBuffer& vbo, const IndexBuffer& ibo, std::vector<Texture>& tex);
    ~Mesh();

    void Draw(Shader& shader);

private:
    void SetupMesh();
};

}
