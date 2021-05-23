#include "VertexBuffer.h"

#include "Common.h"

namespace Core
{

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
    : m_Vertices(vertices)
{
    GLCALL(glGenBuffers(1, &m_RenderId));
    Bind();
    GLCALL(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW));
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