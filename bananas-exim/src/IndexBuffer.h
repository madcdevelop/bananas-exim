#pragma once

namespace Core
{

class IndexBuffer {

    unsigned int m_RenderId;

public:
    IndexBuffer(const void* data, unsigned int size);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

};

}