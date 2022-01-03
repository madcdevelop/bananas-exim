#ifndef GRAPHICS_ENGINE_MESH_H
#define GRAPHICS_ENGINE_MESH_H

#include "../Core/Common.h"
#include "Material.h"
#include "Texture.h"

#include "Shader.h"

namespace GraphicsEngine
{

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureUV;
};

struct Index {
    uint32 positionIndex;
    uint32 textureIndex;
    uint32 normalIndex;

    bool operator<(const Index& other) const {
        return (positionIndex  < other.positionIndex) || 
               (positionIndex == other.positionIndex && 
                textureIndex   < other.textureIndex) ||
               (positionIndex == other.positionIndex && 
                textureIndex  == other.textureIndex && 
                normalIndex    < other.normalIndex);
    }

    bool operator==(const Index& other) const
    {
        return (positionIndex == other.positionIndex &&
                textureIndex  == other.textureIndex &&
                normalIndex   == other.normalIndex);
    }
};

struct Face {
    Index indices[3];
};

class Mesh 
{
public:
    std::string m_name;
    std::vector<Vertex> m_vertices;
    std::vector<uint32> m_indices;
    Material m_material;


private:
    uint32 m_vertexArrayObjectId;
    uint32 m_vertexBufferId;
    uint32 m_indexBufferId;

public:
    Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, Material& material);
    ~Mesh();

    void Draw(Shader& shader);

    void SetupMesh();
};

}

#endif