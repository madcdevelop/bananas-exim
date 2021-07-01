#include "VertexBuffer.h"

#include "../Core/Common.h"

namespace GraphicsEngine
{

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, unsigned int count)
    : m_Count(count)
{
    GLCALL(glGenBuffers(1, &m_RenderId));
    Bind();
    GLCALL(glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RenderId));
}

void VertexBuffer::Bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderId));
}

void VertexBuffer::UnBind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

}