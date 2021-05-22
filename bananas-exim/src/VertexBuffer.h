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

public:
    std::vector<Vertex> m_Vertices;

private:
    unsigned int m_RenderId;

public:
    VertexBuffer(const std::vector<Vertex>& vertices);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

};

}