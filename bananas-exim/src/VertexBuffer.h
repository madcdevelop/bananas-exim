#pragma once

namespace Core
{
    
class VertexBuffer {

    unsigned int m_RenderId;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

};

}