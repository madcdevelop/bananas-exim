#pragma once

#include "Common.h"

namespace Core
{

class IndexBuffer {

    unsigned int m_RenderId;

public:
    IndexBuffer(std::vector<unsigned int> indices);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

};

}