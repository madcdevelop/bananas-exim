#ifndef GRAPHICS_ENGINE_INDEX_BUFFER_H
#define GRAPHICS_ENGINE_INDEX_BUFFER_H

#include "../Core/Common.h"

namespace GraphicsEngine
{

struct Index {
    uint32 positionIndex;
    uint32 textureIndex;
    uint32 normalIndex;
};

struct Face {
    Index indices[3];
};

class IndexBuffer {

public:
    uint32 m_Count;

private:
    uint32 m_RenderId;

public:
    IndexBuffer(const std::vector<uint32>& indices, uint32 count);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

};

}

#endif