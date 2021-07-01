#pragma once

#include "../Core/Common.h"

namespace GraphicsEngine
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
    unsigned int m_Count;

private:
    unsigned int m_RenderId;

public:
    IndexBuffer(const std::vector<unsigned int>& indices, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

};

}