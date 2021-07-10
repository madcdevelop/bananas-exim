#ifndef GRAPHICS_ENGINE_VERTEX_BUFFER_H
#define GRAPHICS_ENGINE_VERTEX_BUFFER_H

#include "../Core/Common.h"

namespace GraphicsEngine
{

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureUV;
};

class VertexBuffer {

public:
    uint32 m_Count;

private:
    uint32 m_RenderId;

public:
    VertexBuffer(const std::vector<Vertex>& vertices, uint32 count);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

};

}

#endif