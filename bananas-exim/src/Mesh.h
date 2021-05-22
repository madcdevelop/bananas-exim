#pragma once

#include "Common.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

namespace Core
{

class Mesh 
{
public:
    VertexBuffer m_VertexBuffer;
    IndexBuffer  m_IndexBuffer;
    std::vector<Texture> m_Textures;
    unsigned int m_VAO;

public:
    Mesh(const VertexBuffer& vbo, const IndexBuffer& ibo, std::vector<Texture>& tex);
    ~Mesh();

private:
    void SetupMesh();
};

}
