#pragma once

#include "Common.h"

namespace Core
{

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureUV;
};

class VertexBuffer {

public:
    unsigned int m_Count;

private:
    unsigned int m_RenderId;

public:
    VertexBuffer(const std::vector<Vertex>& vertices, unsigned int count);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

};

}