#include "VertexBuffer.h"

#include <glad\glad.h>
#include <iostream>

namespace Core
{

    VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    {
        glGenBuffers(1, &m_RenderId);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        std::cout << "Created the Vertex Buffer!\n";
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_RenderId);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RenderId);
    }

    void VertexBuffer::UnBind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

}