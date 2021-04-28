#pragma once

#include "Common.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

namespace Core
{

class Model 
{
public:
    VertexBuffer m_VertexBuffer;
    IndexBuffer  m_IndexBuffer;
    Texture      m_Texture;
    unsigned int m_VAO;

public:
    Model(const VertexBuffer& vbo, const IndexBuffer& ibo, const Texture& tex);
    ~Model();

};

}
