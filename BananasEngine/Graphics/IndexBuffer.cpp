#include "IndexBuffer.h"

#include "../Core/Common.h"

namespace GraphicsEngine
{
    
IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCALL(glGenBuffers(1, &m_RenderId));
    Bind();
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));
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
