#include "IndexBuffer.h"

#include "Common.h"

namespace Core
{
    
    IndexBuffer::IndexBuffer(const void* data, unsigned int size)
    {
        GLCALL(glGenBuffers(1, &m_RenderId));
        Bind();
        GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

        std::cout << "Created the Index Buffer!\n";
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
