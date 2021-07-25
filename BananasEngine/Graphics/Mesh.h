#ifndef GRAPHICS_ENGINE_MESH_H
#define GRAPHICS_ENGINE_MESH_H

#include "../Core/Common.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Texture.h"

#include "Shader.h"

namespace GraphicsEngine
{

class Mesh 
{
public:
    std::string m_Name;
    std::vector<Vertex> m_Vertices;
    std::vector<uint32> m_Indices;
    Material m_Material;

private:
    uint32 m_VAO;

    // TODO: Switch to unique_ptr
    VertexBuffer* m_VertexBuffer;
    IndexBuffer* m_IndexBuffer;

public:
    Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, Material& material);
    ~Mesh();

    void Draw(Shader& shader);

    void SetupMesh();
};

}

#endif