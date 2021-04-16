#include "VertexBuffer.h"

#include "Common.h"

namespace Core
{

    VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    {
        GLCALL(glGenBuffers(1, &m_RenderId));
        Bind();
        GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

        std::cout << "Created the Vertex Buffer!\n";
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