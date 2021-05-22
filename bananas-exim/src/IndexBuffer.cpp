#include "IndexBuffer.h"

#include "Common.h"

namespace Core
{
    
IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices)
    : m_Indices(indices)
{
    GLCALL(glGenBuffers(1, &m_RenderId));
    Bind();
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RenderId));
}

void IndexBuffer::Bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId));
} 

void IndexBuffer::UnBind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

}
