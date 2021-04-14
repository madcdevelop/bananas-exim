#include "IndexBuffer.h"

#include <iostream>

#include "glad\glad.h"

namespace Core
{
    
    IndexBuffer::IndexBuffer(const void* data, unsigned int size)
    {
        glGenBuffers(1, &m_RenderId);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        std::cout << "Created the Index Buffer!\n";
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_RenderId);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId);
    } 

    void IndexBuffer::UnBind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
