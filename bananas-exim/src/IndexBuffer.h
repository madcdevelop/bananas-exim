#pragma once

#include "Common.h"

namespace Core
{

class IndexBuffer {

public:
    std::vector<unsigned int> m_Indices;

private:
    unsigned int m_RenderId;

public:
    IndexBuffer(const std::vector<unsigned int>& indices);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

};

}