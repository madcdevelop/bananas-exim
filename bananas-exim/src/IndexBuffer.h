#pragma once

#include "Common.h"

namespace Core
{

struct Index {
    unsigned int positionIndex;
    unsigned int textureIndex;
    unsigned int normalIndex;
};

struct Face {
    Index indices[3];
};

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