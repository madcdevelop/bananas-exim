#pragma once

#include "Common.h"

namespace Core
{

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureUV;
};

class VertexBuffer {

    unsigned int m_RenderId;

public:
    VertexBuffer(std::vector<Vertex> vertices);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

};

}